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

Result PathUtils::GetPlaceHolderIdFromDirEntry(PlaceHolderId* out, struct dirent* dir_entry) {
    if (strnlen(dir_entry->d_name, 0x30) != 0x24 || strncmp(dir_entry->d_name + 0x20, ".nca", 4) != 0) {
        return ResultNcmInvalidPlaceHolderDirectoryEntry;
    }

    PlaceHolderId placeholder_id = {0};
    char byte_string[2];
    char* end_ptr;
    u64 converted_val;

    for (size_t i = 0; i < sizeof(PlaceHolderId); i++) {
        char* name_char_pair = dir_entry->d_name + i * 2;         
    
        byte_string[0] = name_char_pair[0];
        byte_string[1] = name_char_pair[1];

        converted_val = strtoull(byte_string, &end_ptr, 0x10);
        placeholder_id.uuid[i] = (u8)converted_val;
    }

    *out = placeholder_id;
    return ResultSuccess;
}