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

#include "ncm_make_path.hpp"

#include "ncm_path_utils.hpp"

void ContentPathBuilder::MakeContentPathUnlayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%s", root, content_name) < 0) {
        std::abort();
    }
}

void ContentPathBuilder::MakeContentPathHashByteLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_byte = hash[0];
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_byte, content_name) < 0) {
        std::abort();
    }
}

void ContentPathBuilder::MakeContentPath10BitLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_bytes = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_bytes = (*((u16*)hash) & 0xff00) >> 6;
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_bytes, content_name) < 0) {
        std::abort();
    }
}

void ContentPathBuilder::MakeContentPathDualLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_lower = 0;
    u32 hash_upper = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_lower = (*((u16*)hash) >> 4) & 0x3f;
    hash_upper = (*((u16*)hash) & 0xff00) >> 10;
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%08X/%s", root, hash_upper, hash_lower, content_name) < 0) {
        std::abort();
    }
}

void PlaceHolderPathBuilder::MakePlaceHolderPathUnlayered(char* path_out, PlaceHolderId placeholder_id, const char* root) {
    char placeholder_name[FS_MAX_PATH] = {0};
    PathUtils::GetPlaceHolderFileName(placeholder_name, placeholder_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%.36s", root, placeholder_name) < 0) {
        std::abort();
    }
}

void PlaceHolderPathBuilder::MakePlaceHolderPathHashByteLayered(char* path_out, PlaceHolderId placeholder_id, const char* root) {
    char placeholder_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, placeholder_id.uuid, sizeof(PlaceHolderId));
    hash_byte = hash[0];
    PathUtils::GetPlaceHolderFileName(placeholder_name, placeholder_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_byte, placeholder_name) < 0) {
        std::abort();
    }
}