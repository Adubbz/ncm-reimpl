/*
 * Copyright (c) 2019 Adubbz
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ncm_content.hpp"
#include "fs_utils.hpp"

void PlaceHolderAccessor::GetPlaceHolderPathUncached(char* placeholder_path_out, PlaceHolderId placeholder_id) {
    std::scoped_lock<HosMutex> lock(this->cache_mutex);

    if (memcmp(&placeholder_id.uuid, &InvalidUuid.uuid, sizeof(PlaceHolderId)) != 0) {
        CacheEntry* found_cache = NULL;
        
        for (size_t i = 0; i < PlaceHolderAccessor::MaxCaches; i++) {
            CacheEntry* cache = &this->caches[i];

            if (memcmp(&placeholder_id.uuid, &cache->id.uuid, sizeof(PlaceHolderId)) == 0) {
                found_cache = cache;
                break;
            }
        }

        if (found_cache) {
            /* Flush and close */
            fsync(fileno(found_cache->handle));
            fclose(found_cache->handle);
            std::fill(found_cache->id.uuid, found_cache->id.uuid + sizeof(PlaceHolderId), 0);
        }
    }

    this->GetPlaceHolderPath(placeholder_path_out, placeholder_id);
}

Result PlaceHolderAccessor::Create(PlaceHolderId placeholder_id, size_t size) {
    Result rc = ResultSuccess;
    char placeholder_path[FS_MAX_PATH] = {0};

    this->GetPlaceHolderPath(placeholder_path, placeholder_id);

    if (R_FAILED(rc = FsUtils::EnsureParentDirectoryRecursively(placeholder_path))) {
        return rc;
    }

    std::fill(placeholder_path, placeholder_path + FS_MAX_PATH, 0);
    this->GetPlaceHolderPathUncached(placeholder_path, placeholder_id);

    if (R_FAILED(rc = FsUtils::CreateFile(placeholder_path, size, true)) && rc != ResultFsPathAlreadyExists) {
        return rc;
    }

    if (rc == ResultFsPathAlreadyExists) {
        return ResultNcmPlaceHolderAlreadyExists;
    }

    return rc;
}

Result PlaceHolderAccessor::Delete(PlaceHolderId placeholder_id) {
    Result rc = ResultSuccess;
    char placeholder_path[FS_MAX_PATH] = {0};

    this->GetPlaceHolderPathUncached(placeholder_path, placeholder_id);

    if (R_FAILED(rc = fsdevDeleteDirectoryRecursively(placeholder_path)) && rc != ResultFsPathNotFound) {
        return rc;
    }

    if (rc == ResultFsPathNotFound) {
        return ResultNcmPlaceHolderNotFound;
    }

    return rc;
}

Result PlaceHolderAccessor::Open(FILE** out_handle, PlaceHolderId placeholder_id) {
    if (this->LoadFromCache(out_handle, placeholder_id)) {
        return ResultSuccess;
    }
    char placeholder_path[FS_MAX_PATH] = {0};

    this->GetPlaceHolderPath(placeholder_path, placeholder_id);
    errno = 0;
    *out_handle = fopen(placeholder_path, "w+");

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    return ResultSuccess;
}

bool PlaceHolderAccessor::LoadFromCache(FILE** out_handle, PlaceHolderId placeholder_id) {
    std::scoped_lock<HosMutex> lk(this->cache_mutex);
    CacheEntry *entry = this->FindInCache(placeholder_id);
    if (entry == nullptr) {
        return false;
    }
    entry->id = InvalidUuid;
    *out_handle = entry->handle;
    return true;
}

PlaceHolderAccessor::CacheEntry *PlaceHolderAccessor::FindInCache(PlaceHolderId placeholder_id) {
    if (memcmp(&placeholder_id.uuid, &InvalidUuid.uuid, sizeof(PlaceHolderId)) == 0) {
        return nullptr;
    }
    for (size_t i = 0; i < MaxCaches; i++) {
        if (memcmp(&placeholder_id.uuid, &this->caches[i].id.uuid, sizeof(PlaceHolderId)) == 0) {
            return &this->caches[i];
        }
    }
    return nullptr;
}

void PlaceHolderAccessor::StoreToCache(FILE* handle, PlaceHolderId placeholder_id) {
    std::scoped_lock<HosMutex> lk(this->cache_mutex);
    CacheEntry* cache = nullptr;

    /* Find an empty cache */
    for (size_t i = 0; i < MaxCaches; i++) {
        if (memcmp(&placeholder_id.uuid, &InvalidUuid.uuid, sizeof(PlaceHolderId)) != 0) {
            cache = &this->caches[i];
            break;
        }
    }

    /* No empty caches found. Let's clear cache 0. */
    if (cache == nullptr) {
        cache = &this->caches[0];

        /* Flush and close */
        fsync(fileno(cache->handle));
        fclose(cache->handle);
        cache->id = InvalidUuid;
    }

    cache->id = placeholder_id;
    cache->handle = handle;
    cache->counter = this->cur_counter;
    this->cur_counter++;
}

void PlaceHolderAccessor::ClearAllCaches() {
    for (size_t i = 0; i < MaxCaches; i++) {
        CacheEntry* cache = &this->caches[i];

        if (memcmp(&cache->id.uuid, &InvalidUuid.uuid, sizeof(PlaceHolderId)) != 0) {
            fsync(fileno(cache->handle));
            fclose(cache->handle);
            cache->id = InvalidUuid;
        }
    }
}

void PathBuilder::MakeContentPathUnlayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    GetContentFileName(content_name, content_id);
    snprintf(path_out, FS_MAX_PATH-1, "%s/%.36s", root, content_name);
}

void PathBuilder::MakeContentPathHashByteLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_byte = hash[0];
    GetContentFileName(content_name, content_id);
    snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%.36s", root, hash_byte, content_name);
}

void PathBuilder::MakeContentPath10BitLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_bytes = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_bytes = (*((u16*)hash) & 0xff00) >> 6;
    GetContentFileName(content_name, content_id);
    snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%.36s", root, hash_bytes, content_name);
}

void PathBuilder::MakeContentPathDualLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_lower = 0;
    u32 hash_upper = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_lower = (*((u16*)hash) >> 4) & 0x3f;
    hash_upper = (*((u16*)hash) & 0xff00) >> 10;
    GetContentFileName(content_name, content_id);
    snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%08X/%.36s", root, hash_upper, hash_lower, content_name);
}

void PathBuilder::MakePlaceHolderPathUnlayered(char* path_out, PlaceHolderId placeholder_id, const char* root) {
    char placeholder_name[FS_MAX_PATH] = {0};
    GetPlaceHolderFileName(placeholder_name, placeholder_id);
    snprintf(path_out, FS_MAX_PATH-1, "%s/%.36s", root, placeholder_name);
}

void PathBuilder::MakePlaceHolderPathHashByteLayered(char* path_out, PlaceHolderId placeholder_id, const char* root) {
    char placeholder_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, placeholder_id.uuid, sizeof(PlaceHolderId));
    hash_byte = hash[0];
    GetPlaceHolderFileName(placeholder_name, placeholder_id);
    snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%.36s", root, hash_byte, placeholder_name);
}

void PathBuilder::GetStringFromContentId(char* out, ContentId content_id) {
    for (size_t i = 0; i < sizeof(ContentId); i++) {
        snprintf(out+i, 3, "%02x", content_id.uuid[i]);
    }
}

void PathBuilder::GetContentFileName(char* out, ContentId content_id) {
    char content_name[sizeof(ContentId)*2+1] = {0};
    GetStringFromContentId(content_name, content_id);
    snprintf(out, FS_MAX_PATH-1, "%s%s", content_name, ".nca");
}

void PathBuilder::GetStringFromPlaceHolderId(char* out, PlaceHolderId placeholder_id) {
    for (size_t i = 0; i < sizeof(PlaceHolderId); i++) {
        snprintf(out+i, 3, "%02x", placeholder_id.uuid[i]);
    }
}

void PathBuilder::GetPlaceHolderFileName(char* out, PlaceHolderId placeholder_id) {
    char placeholder_name[sizeof(PlaceHolderId)*2+1] = {0};
    GetStringFromPlaceHolderId(placeholder_name, placeholder_id);
    snprintf(out, FS_MAX_PATH-1, "%s%s", placeholder_name, ".nca");
}

unsigned int PathBuilder::GetDirLevelForContentPathFunc(MakeContentPathFunc* content_path_func) {
    if (content_path_func == reinterpret_cast<MakeContentPathFunc*>(MakeContentPathUnlayered)) {
        return 1;
    }
    else if (content_path_func == reinterpret_cast<MakeContentPathFunc*>(MakeContentPathHashByteLayered)) {
        return 2;
    }
    else if (content_path_func == reinterpret_cast<MakeContentPathFunc*>(MakeContentPath10BitLayered)) {
        return 2;
    }
    else if (content_path_func == reinterpret_cast<MakeContentPathFunc*>(MakeContentPathDualLayered)) {
        return 3;
    }

    std::abort();
}

unsigned int PathBuilder::GetDirLevelForPlaceHolderPathFunc(MakePlaceHolderPathFunc* placeholder_path_func) {
    if (placeholder_path_func == reinterpret_cast<MakePlaceHolderPathFunc*>(MakePlaceHolderPathUnlayered)) {
        return 1;
    }
    else if (placeholder_path_func == reinterpret_cast<MakePlaceHolderPathFunc*>(MakePlaceHolderPathHashByteLayered)) {
        return 2;
    }

    std::abort();
}