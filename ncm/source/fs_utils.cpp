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

#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include "fs_utils.hpp"

Result FsUtils::EnsureParentDirectoryRecursively(const char* path) {
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