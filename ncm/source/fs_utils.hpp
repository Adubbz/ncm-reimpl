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

class FsUtils {
    public:
        /* Create all parent directories for a file path */
        static Result EnsureParentDirectoryRecursively(const char* path);
        /* Create a file. Flush immediately is currently unused, but included to maintain consistency with N's impl */
        static Result CreateFile(const char* path, size_t size, bool flush_immediately);

        template<typename F>
        Result TraverseDirectory(bool* out_should_continue, const char* root_path, int max_level, F f) {
            Result rc;
            DIR *dir;
            struct dirent* dir_entry = nullptr;
            if (max_level < 1) {
                return ResultSuccess;
            }
            
            if ((dir = opendir(root_path)) == nullptr) {
                return fsdevGetLastResult();
            }
            ON_SCOPE_EXIT { closedir(dir); };

            while ((dir_entry = readdir(dir)) != nullptr) {
                if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
                    continue;
                }

                char current_path[FS_MAX_PATH];
                snprintf(current_path, sizeof(current_path), "%s/%s", root_path, dir_entry->d_name);

                bool should_continue = true;
                if (R_FAILED((rc = f(&should_continue, current_path, dir_entry)))) {
                    return rc;
                }

                /* If the provided function wishes to terminate immediately, we should respect it. */
                if (!should_continue) {
                    *out_should_continue = false;
                    break;
                }

                if (dir_entry->d_type == DT_DIR) {
                    if (R_FAILED((rc = TraverseDirectory(&should_continue, current_path, max_level-1, f)))) {
                        return rc;
                    }

                    if (!should_continue) {
                        *out_should_continue = false;
                        break;
                    }
                }
            }

            return ResultSuccess;
        };
};