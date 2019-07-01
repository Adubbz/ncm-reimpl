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

namespace sts::ncm {

    Result ContentManagerService::CreateContentStorage(StorageId storage_id) {
        return ResultKernelConnectionClosed;
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