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

    if (memcmp(&placeholder_id, &InvalidUuid, sizeof(PlaceHolderId)) != 0) {
        PlaceHolderAccessorCache* found_cache = NULL;
        
        for (size_t i = 0; i < PlaceHolderAccessor::MaxCaches; i++) {
            PlaceHolderAccessorCache* cache = &this->caches[i];

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