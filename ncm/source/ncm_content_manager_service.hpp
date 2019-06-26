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

namespace sts::ncm {

    class ContentManagerService final : public IServiceObject {
        private:
            enum class CommandId {
                CreateContentStorage = 0,
                CreateContentMetaDatabase = 1,
                VerifyContentStorage = 2,
                VerifyContentMetaDatabase = 3,
                OpenContentStorage = 4,
                OpenContentMetaDatabase = 5,
                CloseContentStorageForcibly = 6,
                CloseContentMetaDatabaseForcibly = 7,
                CleanupContentMetaDatabase = 8,
                ActivateContentStorage = 9,
                InactivateContentStorage = 10,
                ActivateContentMetaDatabase = 11,
                InactivateContentMetaDatabase = 12,
            };

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
            DEFINE_SERVICE_DISPATCH_TABLE {
                /* 1.0.0 only */
                MakeServiceCommandMeta<CommandId::CloseContentStorageForcibly, &ContentManagerService::CloseContentStorageForcibly, FirmwareVersion_100, FirmwareVersion_100>(),
                MakeServiceCommandMeta<CommandId::CloseContentMetaDatabaseForcibly, &ContentManagerService::CloseContentMetaDatabaseForcibly, FirmwareVersion_100, FirmwareVersion_100>(),

                /* 1.0.0- */
                MakeServiceCommandMeta<CommandId::CreateContentStorage, &ContentManagerService::CreateContentStorage>(),
                MakeServiceCommandMeta<CommandId::CreateContentMetaDatabase, &ContentManagerService::CreateContentMetaDatabase>(),
                MakeServiceCommandMeta<CommandId::VerifyContentStorage, &ContentManagerService::VerifyContentStorage>(),
                MakeServiceCommandMeta<CommandId::VerifyContentMetaDatabase, &ContentManagerService::VerifyContentMetaDatabase>(),
                MakeServiceCommandMeta<CommandId::OpenContentStorage, &ContentManagerService::OpenContentStorage>(),
                MakeServiceCommandMeta<CommandId::OpenContentMetaDatabase, &ContentManagerService::OpenContentMetaDatabase>(),
                MakeServiceCommandMeta<CommandId::CleanupContentMetaDatabase, &ContentManagerService::CleanupContentMetaDatabase>(),
            
                /* 2.0.0- */
                MakeServiceCommandMeta<CommandId::ActivateContentStorage, &ContentManagerService::ActivateContentStorage, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::InactivateContentStorage, &ContentManagerService::InactivateContentStorage, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::ActivateContentMetaDatabase, &ContentManagerService::ActivateContentMetaDatabase, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::InactivateContentMetaDatabase, &ContentManagerService::InactivateContentMetaDatabase, FirmwareVersion_200>(),
            };
    };

}