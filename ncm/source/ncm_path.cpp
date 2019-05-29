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

#include "ncm_path.hpp"

void PathUtils::MakeContentPathUnlayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%s", root, content_name) < 0) {
        std::abort();
    }
}

void PathUtils::MakeContentPathHashByteLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_byte = hash[0];
    GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_byte, content_name) < 0) {
        std::abort();
    }
}

void PathUtils::MakeContentPath10BitLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_bytes = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_bytes = (*((u16*)hash) & 0xff00) >> 6;
    GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_bytes, content_name) < 0) {
        std::abort();
    }
}

void PathUtils::MakeContentPathDualLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_lower = 0;
    u32 hash_upper = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_lower = (*((u16*)hash) >> 4) & 0x3f;
    hash_upper = (*((u16*)hash) & 0xff00) >> 10;
    GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%08X/%s", root, hash_upper, hash_lower, content_name) < 0) {
        std::abort();
    }
}

void PathUtils::MakePlaceHolderPathUnlayered(char* path_out, PlaceHolderId placeholder_id, const char* root) {
    char placeholder_name[FS_MAX_PATH] = {0};
    GetPlaceHolderFileName(placeholder_name, placeholder_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%.36s", root, placeholder_name) < 0) {
        std::abort();
    }
}

void PathUtils::MakePlaceHolderPathHashByteLayered(char* path_out, PlaceHolderId placeholder_id, const char* root) {
    char placeholder_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, placeholder_id.uuid, sizeof(PlaceHolderId));
    hash_byte = hash[0];
    GetPlaceHolderFileName(placeholder_name, placeholder_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_byte, placeholder_name) < 0) {
        std::abort();
    }
}

void PathUtils::GetStringFromContentId(char* out, ContentId content_id) {
    for (size_t i = 0; i < sizeof(ContentId); i++) {
        snprintf(out+i, 3, "%02x", content_id.uuid[i]);
    }
}

void PathUtils::GetContentFileName(char* out, ContentId content_id) {
    char content_name[sizeof(ContentId)*2+1] = {0};
    GetStringFromContentId(content_name, content_id);
    snprintf(out, FS_MAX_PATH-1, "%s%s", content_name, ".nca");
}

void PathUtils::GetStringFromPlaceHolderId(char* out, PlaceHolderId placeholder_id) {
    for (size_t i = 0; i < sizeof(PlaceHolderId); i++) {
        snprintf(out+i, 3, "%02x", placeholder_id.uuid[i]);
    }
}

void PathUtils::GetPlaceHolderFileName(char* out, PlaceHolderId placeholder_id) {
    char placeholder_name[sizeof(PlaceHolderId)*2+1] = {0};
    GetStringFromPlaceHolderId(placeholder_name, placeholder_id);
    snprintf(out, FS_MAX_PATH-1, "%s%s", placeholder_name, ".nca");
}

unsigned int PathUtils::GetDirLevelForContentPathFunc(MakeContentPathFunc content_path_func) {
    if (content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPathUnlayered)) {
        return 1;
    }
    else if (content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPathHashByteLayered)) {
        return 2;
    }
    else if (content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPath10BitLayered)) {
        return 2;
    }
    else if (content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPathDualLayered)) {
        return 3;
    }

    std::abort();
}

unsigned int PathUtils::GetDirLevelForPlaceHolderPathFunc(MakePlaceHolderPathFunc placeholder_path_func) {
    if (placeholder_path_func == reinterpret_cast<MakePlaceHolderPathFunc>(MakePlaceHolderPathUnlayered)) {
        return 1;
    }
    else if (placeholder_path_func == reinterpret_cast<MakePlaceHolderPathFunc>(MakePlaceHolderPathHashByteLayered)) {
        return 2;
    }

    std::abort();
}

Result PathUtils::GetPlaceHolderIdFromDirEntry(PlaceHolderId* out, struct dirent* dir_entry) {
    //TODO:
    return ResultKernelConnectionClosed;
}