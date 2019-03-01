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
#include "ncm_content_meta_database.hpp"
#include "ncm_content_storage.hpp"

enum ContentManagerCmd : u32
{
    ContentManager_Cmd_CreateContentStorage = 0,
    ContentManager_Cmd_CreateContentMetaDatabase = 1,
    ContentManager_Cmd_VerifyContentStorage = 2,
    ContentManager_Cmd_VerifyContentMetaDatabase = 3,
    ContentManager_Cmd_OpenContentStorage = 4,
    ContentManager_Cmd_OpenContentMetaDatabase = 5,
    ContentManager_Cmd_CloseContentStorageForcibly = 6,
    ContentManager_Cmd_CloseContentMetaDatabaseForcibly = 7,
    ContentManager_Cmd_CleanupContentMetaDatabase = 8,
    ContentManager_Cmd_ActivateContentStorage = 9,
    ContentManager_Cmd_InactivateContentStorage = 10,
    ContentManager_Cmd_ActivateContentMetaDatabase = 11,
    ContentManager_Cmd_InactivateContentMetaDatabase = 12,
};

class ContentManagerService final : public IServiceObject
{
    private:
        Result CreateContentStorage(FsStorageId storage_id);
        Result CreateContentMetaDatabase(FsStorageId storage_id);
        Result VerifyContentStorage(FsStorageId storage_id);
        Result VerifyContentMetaDatabase(FsStorageId storage_id);
        Result OpenContentStorage(Out<std::shared_ptr<ContentStorageInterface>> out, FsStorageId storage_id);
        Result OpenContentMetaDatabase(Out<std::shared_ptr<ContentMetaDatabaseInterface>> out, FsStorageId storage_id);
        Result CloseContentStorageForcibly(FsStorageId storage_id);
        Result CloseContentMetaDatabaseForcibly(FsStorageId storage_id);
        Result CleanupContentMetaDatabase(FsStorageId storage_id);
        Result ActivateContentStorage(FsStorageId storage_id);
        Result InactivateContentStorage(FsStorageId storage_id);
        Result ActivateContentMetaDatabase(FsStorageId storage_id);
        Result InactivateContentMetaDatabase(FsStorageId storage_id);

    public:
        DEFINE_SERVICE_DISPATCH_TABLE
        {
            /* 1.0.0 only */
            MakeServiceCommandMeta<ContentManager_Cmd_CloseContentStorageForcibly, &ContentManagerService::CloseContentStorageForcibly, FirmwareVersion_100, FirmwareVersion_100>(),
            MakeServiceCommandMeta<ContentManager_Cmd_CloseContentMetaDatabaseForcibly, &ContentManagerService::CloseContentMetaDatabaseForcibly, FirmwareVersion_100, FirmwareVersion_100>(),

            /* 1.0.0- */
            MakeServiceCommandMeta<ContentManager_Cmd_CreateContentStorage, &ContentManagerService::CreateContentStorage>(),
            MakeServiceCommandMeta<ContentManager_Cmd_CreateContentMetaDatabase, &ContentManagerService::CreateContentMetaDatabase>(),
            MakeServiceCommandMeta<ContentManager_Cmd_VerifyContentStorage, &ContentManagerService::VerifyContentStorage>(),
            MakeServiceCommandMeta<ContentManager_Cmd_VerifyContentMetaDatabase, &ContentManagerService::VerifyContentMetaDatabase>(),
            MakeServiceCommandMeta<ContentManager_Cmd_OpenContentStorage, &ContentManagerService::OpenContentStorage>(),
            MakeServiceCommandMeta<ContentManager_Cmd_OpenContentMetaDatabase, &ContentManagerService::OpenContentMetaDatabase>(),
            MakeServiceCommandMeta<ContentManager_Cmd_CleanupContentMetaDatabase, &ContentManagerService::CleanupContentMetaDatabase>(),
        
            /* 2.0.0- */
            MakeServiceCommandMeta<ContentManager_Cmd_ActivateContentStorage, &ContentManagerService::ActivateContentStorage, FirmwareVersion_200>(),
            MakeServiceCommandMeta<ContentManager_Cmd_InactivateContentStorage, &ContentManagerService::InactivateContentStorage, FirmwareVersion_200>(),
            MakeServiceCommandMeta<ContentManager_Cmd_ActivateContentMetaDatabase, &ContentManagerService::ActivateContentMetaDatabase, FirmwareVersion_200>(),
            MakeServiceCommandMeta<ContentManager_Cmd_InactivateContentMetaDatabase, &ContentManagerService::InactivateContentMetaDatabase, FirmwareVersion_200>(),
        };
};