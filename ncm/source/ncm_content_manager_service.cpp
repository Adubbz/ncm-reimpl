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
    return 0xF601;
}

Result ContentManagerService::CreateContentMetaDatabase(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::VerifyContentStorage(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::VerifyContentMetaDatabase(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::OpenContentStorage(Out<std::shared_ptr<ContentStorageInterface>> out, FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::OpenContentMetaDatabase(Out<std::shared_ptr<ContentMetaDatabaseInterface>> out, FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::CloseContentStorageForcibly(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::CloseContentMetaDatabaseForcibly(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::CleanupContentMetaDatabase(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::ActivateContentStorage(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::InactivateContentStorage(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::ActivateContentMetaDatabase(FsStorageId storage_id)
{
    return 0xF601;
}

Result ContentManagerService::InactivateContentMetaDatabase(FsStorageId storage_id)
{
    return 0xF601;
}