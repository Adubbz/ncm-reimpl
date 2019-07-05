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

        if (storage_id == StorageId::None || static_cast<u8>(storage_id) == 6) {
            return ResultNcmUnknownStorage;
        }
        
        ContentStorageEntry* found_entry = nullptr;
        FsFileSystem fs;

        for (size_t i = 0; i < ContentManagerService::MaxContentStorageEntries; i++) {
            ContentStorageEntry* entry = &this->content_storage_entries[i];

            if (entry->storage_id == storage_id) {
                R_TRY(fsOpenContentStorageFileSystem(&fs, entry->content_storage_id));
                found_entry = entry;
                break;
            }
        }

        if (!found_entry) {
            return ResultNcmUnknownStorage;
        }

        if (fsdevMountDevice(found_entry->mount_point, fs) == -1) {
            std::abort();
        }

        ON_SCOPE_EXIT {
            if (fsdevUnmountDevice(found_entry->mount_point) == -1) {
                std::abort();
            }
        };

        R_TRY(EnsureDirectoryRecursively(found_entry->root_path));
        R_TRY(EnsureContentAndPlaceHolderRoot(found_entry->root_path));

        return ResultSuccess;
    }

    Result ContentManagerService::CreateContentMetaDatabase(StorageId storage_id) {
        if (storage_id == StorageId::None || storage_id == StorageId::GameCard || static_cast<u8>(storage_id) == 6) {
            return ResultNcmUnknownStorage;
        }

        ContentMetaDBEntry* found_entry = nullptr;

        for (size_t i = 0; i < ContentManagerService::MaxContentStorageEntries; i++) {
            ContentMetaDBEntry* entry = &this->content_meta_entries[i];

            if (entry->storage_id == storage_id) {
                found_entry = entry;
                break;
            }
        }

        if (!found_entry) {
            return ResultNcmUnknownStorage;
        }

        /* N doesn't bother checking the result of this. */
        fsDisableAutoSaveDataCreation();

        R_TRY_CATCH(MountSystemSaveData(found_entry->mount_point, found_entry->save_meta.space_id, found_entry->save_meta.id)) {
            R_CATCH(ResultFsTargetNotFound ) {
                R_TRY(fsCreate_SystemSaveData(found_entry->save_meta.space_id, found_entry->save_meta.id, found_entry->save_meta.size, found_entry->save_meta.journal_size, found_entry->save_meta.flags));
                R_TRY(MountSystemSaveData(found_entry->mount_point, found_entry->save_meta.space_id, found_entry->save_meta.id));
            }
        } R_END_TRY_CATCH;

        ON_SCOPE_EXIT {
            if (fsdevUnmountDevice(found_entry->mount_point) == -1) {
                std::abort();
            }
        };

        R_TRY(EnsureDirectoryRecursively(found_entry->mount_point));
        R_TRY(fsdevCommitDevice(found_entry->mount_point));

        return ResultSuccess;
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