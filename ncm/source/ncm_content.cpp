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

void ContentUtils::GetPlaceHolderPathUncached(PlaceHolderAccessor* accessor, char* placeholder_path_out, PlaceHolderId placeholder_id) {
    std::scoped_lock<HosMutex> lock(accessor->cache_mutex);

    if (memcmp(&placeholder_id, &InvalidUuid, sizeof(PlaceHolderId)) != 0) {
        PlaceHolderAccessorCache* found_cache = NULL;
        
        for (size_t i = 0; i < PlaceHolderAccessor::MaxCaches; i++) {
            PlaceHolderAccessorCache* cache = &accessor->caches[i];

            if (memcmp(&placeholder_id, &cache->id, sizeof(PlaceHolderId)) == 0) {
                found_cache = cache;
                break;
            }
        }

        if (found_cache) {
            /* Flush and close */
            fsync(fileno(found_cache->handle));
            fclose(found_cache->handle);
            std::fill(found_cache->id.c, found_cache->id.c + sizeof(PlaceHolderId), 0);
        }
    }

    char placeholder_root_path[FS_MAX_PATH] = {0};
    /* TODO: Replace with BoundedString? */
    snprintf(placeholder_root_path, FS_MAX_PATH, "%s%s", accessor->root_path, "/placehld");
    accessor->make_placeholder_path_func(placeholder_path_out, placeholder_id, placeholder_root_path);
}

Result ContentUtils::CreatePlaceHolderFile(PlaceHolderAccessor* accessor, PlaceHolderId placeholder_id, size_t size) {
    Result rc = ResultSuccess;
    char placeholder_root_path[FS_MAX_PATH] = {0};
    char placeholder_path[FS_MAX_PATH] = {0};

    /* TODO: Replace with BoundedString? */
    snprintf(placeholder_root_path, FS_MAX_PATH, "%s%s", accessor->root_path, "/placehld");
    accessor->make_placeholder_path_func(placeholder_path, placeholder_id, placeholder_root_path);

    if (R_FAILED(rc = FsUtils::EnsureParentDirectoryRecursively(placeholder_path))) {
        return rc;
    }

    std::fill(placeholder_path, placeholder_path + FS_MAX_PATH, 0);
    GetPlaceHolderPathUncached(accessor, placeholder_path, placeholder_id);

    if (R_FAILED(rc = FsUtils::CreateFile(placeholder_path, size, true)) && rc != ResultFsPathAlreadyExists) {
        return rc;
    }

    if (rc == ResultFsPathAlreadyExists) {
        return ResultNcmPlaceHolderAlreadyExists;
    }

    return rc;
}

Result ContentUtils::DeletePlaceHolderDirectory(PlaceHolderAccessor* accessor, PlaceHolderId placeholder_id) {
    Result rc = ResultSuccess;
    char placeholder_path[FS_MAX_PATH] = {0};

    GetPlaceHolderPathUncached(accessor, placeholder_path, placeholder_id);

    if (R_FAILED(rc = fsdevDeleteDirectoryRecursively(placeholder_path)) && rc != ResultFsPathNotFound) {
        return rc;
    }

    if (rc == ResultFsPathNotFound) {
        return ResultNcmPlaceHolderNotFound;
    }

    return rc;
}