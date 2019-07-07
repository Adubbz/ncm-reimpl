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

#include "ncm_utils.hpp"

namespace sts::ncm {

    void GetStringFromContentId(char* out, ContentId content_id) {
        for (size_t i = 0; i < sizeof(ContentId); i++) {
            snprintf(out+i, 3, "%02x", content_id.uuid[i]);
        }
    }

    void GetStringFromPlaceHolderId(char* out, PlaceHolderId placeholder_id) {
        for (size_t i = 0; i < sizeof(PlaceHolderId); i++) {
            snprintf(out+i, 3, "%02x", placeholder_id.uuid[i]);
        }
    }

    Result GetPlaceHolderIdFromDirEntry(PlaceHolderId* out, struct dirent* dir_entry) {
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

    std::optional<ContentId> GetContentIdFromString(const char* str, size_t len) {
        ContentId content_id = {0};

        if (len < 0x20) {
            return std::nullopt;
        }

        char byte_string[2];
        char* end_ptr;
        u64 converted_val;

        for (size_t i = 0; i < sizeof(ContentId); i++) {
            const char* char_par = str + i * 2;         
        
            byte_string[0] = char_par[0];
            byte_string[1] = char_par[1];

            converted_val = strtoull(byte_string, &end_ptr, 0x10);
            content_id.uuid[i] = (u8)converted_val;
        }

        return std::optional<ContentId>(content_id);
    }

    Result EnsureDirectoryRecursively(const char* dir_path) {
        return EnsureRecursively(dir_path, true);
    }

    Result EnsureRecursively(const char* path, bool is_dir) {
        size_t path_len = strlen(path);
        char working_path_buf[FS_MAX_PATH] = {0};

        if (path_len + 1 < FS_MAX_PATH) {
            strncpy(working_path_buf + 1, path, FS_MAX_PATH-1);

            if (path_len != 0) {
                for (size_t i = 0; i < path_len; i++) {
                    if (i != 0 && working_path_buf[i + 1] == '/' && working_path_buf[i] != ':') {
                        /* Wipe the errno to prevent cross-contamination */
                        errno = 0;
                        /* Temporarily make the path terminate before the '/' */
                        working_path_buf[i + 1] = 0;
                        mkdir(working_path_buf + 1, S_IRWXU);

                        if (errno != 0) {
                            R_TRY_CATCH(fsdevGetLastResult()) {
                                R_CATCH(ResultFsPathAlreadyExists) {
                                    /* If the path already exists, that's okay. Anything else is an error. */
                                }
                            } R_END_TRY_CATCH;
                        }

                        /* Restore the path to its former state */
                        working_path_buf[i + 1] = '/';
                    }
                }
            }
        } else {
            return ResultNcmAllocationFailed;
        }

        return ResultSuccess;
    }

    Result EnsureParentDirectoryRecursively(const char* path) {
        return EnsureRecursively(path, false);
    }

    static u32 g_mount_index = 0;
    static HosMutex g_mount_index_lock;

    MountName CreateUniqueMountName() {
        std::scoped_lock<HosMutex> lk(g_mount_index_lock);
        MountName mount_name;
        g_mount_index++;
        snprintf(mount_name.name, sizeof(MountName), "@ncm%08x", g_mount_index);
        return mount_name;
    }

}