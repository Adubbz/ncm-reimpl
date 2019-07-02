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

#include "ncm_content_manager_service.hpp"
#include "ncm_utils.hpp"

namespace sts::ncm {

    Result ContentManagerService::CreateContentStorage(StorageId storage_id) {
        std::scoped_lock<HosMutex> lk(this->mutex);
        ContentStorageEntry* found_entry = nullptr;
        FsFileSystem fs;

        if (storage_id != StorageId::None && static_cast<u8>(storage_id) != 6) {
            for (size_t i = 0; i < ContentManagerService::MaxContentStorageEntries; i++) {
                ContentStorageEntry* entry = &this->content_storage_entries[i];

                if (entry->storage_id == storage_id) {
                    R_TRY(fsOpenContentStorageFileSystem(&fs, entry->content_storage_id));
                    if (fsdevMountDevice(entry->mount_point, fs) == -1) {
                        std::abort();
                    }
                    found_entry = entry;
                    break;
                }
            }
        }

        if (!found_entry) {
            return ResultNcmUnknownStorage;
        }

        auto fs_guard = SCOPE_GUARD {
            if (fsdevUnmountDevice(found_entry->mount_point) == -1) {
                std::abort();
            }
        };

        R_TRY(EnsureDirectoryRecursively(found_entry->root_path));
        R_TRY(EnsureContentAndPlaceHolderRoot(found_entry->root_path));

        fs_guard.Cancel();
        return ResultSuccess;
    }

    Result ContentManagerService::CreateContentMetaDatabase(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::VerifyContentStorage(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::VerifyContentMetaDatabase(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::OpenContentStorage(Out<std::shared_ptr<ContentStorageInterface>> out, StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::OpenContentMetaDatabase(Out<std::shared_ptr<ContentMetaDatabaseInterface>> out, StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::CloseContentStorageForcibly(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::CloseContentMetaDatabaseForcibly(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::CleanupContentMetaDatabase(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::ActivateContentStorage(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::InactivateContentStorage(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::ActivateContentMetaDatabase(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentManagerService::InactivateContentMetaDatabase(StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

}