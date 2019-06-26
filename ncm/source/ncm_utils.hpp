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

#pragma once
#include <switch.h>
#include <stratosphere.hpp>
#include <sys/dirent.h>

#include "ncm_types.hpp"

namespace sts::ncm {

    void GetStringFromContentId(char* out, ContentId content_id);
    void GetStringFromPlaceHolderId(char* out, PlaceHolderId placeholder_id);

    Result GetPlaceHolderIdFromDirEntry(PlaceHolderId* out, struct dirent* dir_entry);
    std::optional<ContentId> GetContentIdFromString(const char* str, size_t len);

    /* Create all parent directories for a file path */
    Result EnsureParentDirectoryRecursively(const char* path);
    template<typename F>
    Result TraverseDirectory(const char* root_path, int max_level, F f) {
        bool should_continue = false;
        return TraverseDirectory(&should_continue, root_path, max_level, f);
    }

    template<typename F>
    Result TraverseDirectory(bool* out_should_continue, const char* root_path, int max_level, F f) {
        DIR *dir;
        struct dirent* dir_entry = nullptr;
        if (max_level < 1) {
            return ResultSuccess;
        }
        
        bool retry_dir_read = true;
        while (retry_dir_read) {
            retry_dir_read = false;

            if ((dir = opendir(root_path)) == nullptr) {
                return fsdevGetLastResult();
            }
            ON_SCOPE_EXIT { closedir(dir); };

            while ((dir_entry = readdir(dir)) != nullptr) {
                if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
                    continue;
                }

                char current_path[FS_MAX_PATH];
                if (snprintf(current_path, FS_MAX_PATH-1, "%s/%s", root_path, dir_entry->d_name) < 0) {
                    std::abort();
                }
                
                bool should_continue = true;
                bool should_retry_dir_read = false;
                R_TRY(f(&should_continue, &should_retry_dir_read, current_path, dir_entry));

                /* If the provided function wishes to terminate immediately, we should respect it. */
                if (!should_continue) {
                    *out_should_continue = false;
                    return ResultSuccess;
                }
                if (should_retry_dir_read) {
                    retry_dir_read = true;
                    break;
                }

                if (dir_entry->d_type == DT_DIR) {
                    R_TRY(TraverseDirectory(&should_continue, current_path, max_level-1, f));

                    if (!should_continue) {
                        *out_should_continue = false;
                        return ResultSuccess;
                    }
                }
            }
        }

        return ResultSuccess;
    };
    
};