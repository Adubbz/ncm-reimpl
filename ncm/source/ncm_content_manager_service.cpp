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

Result ContentManagerService::CreateContentStorage(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::CreateContentMetaDatabase(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::VerifyContentStorage(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::VerifyContentMetaDatabase(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::OpenContentStorage(Out<std::shared_ptr<ContentStorageInterface>> out, FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::OpenContentMetaDatabase(Out<std::shared_ptr<ContentMetaDatabaseInterface>> out, FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::CloseContentStorageForcibly(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::CloseContentMetaDatabaseForcibly(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::CleanupContentMetaDatabase(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::ActivateContentStorage(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::InactivateContentStorage(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::ActivateContentMetaDatabase(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentManagerService::InactivateContentMetaDatabase(FsStorageId storage_id)
{
    return ResultKernelConnectionClosed;
}