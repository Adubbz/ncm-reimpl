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
            struct ContentStorageEntry {
                char mount_point[16];
                char root_path[128];
                StorageId storage_id;
                FsContentStorageId content_storage_id;
                ContentStorageInterface* content_storage;
            };

            struct SaveDataMeta {
                u64 id;
                u64 size;
                u64 journal_size;
                u32 flags;
                u32 space_id;
            };

            struct ContentMetaDBEntry {
                char mount_name[16];
                char meta_path[128];
                StorageId storage_id;
                SaveDataMeta save_meta;
                ContentMetaDatabaseInterface* content_meta_database;
                void* kvdb_store; // TODO: Replace this when implemented
                u32 max_content_metas;
            };

        private:
            HosMutex mutex;
            bool initialized;
            ContentStorageEntry content_storage_entries[8];
            ContentMetaDBEntry content_meta_entries[8];
            u32 num_content_storage_entries;
            u32 num_content_meta_entries;

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

            Result CreateContentStorage(StorageId storage_id);
            Result CreateContentMetaDatabase(StorageId storage_id);
            Result VerifyContentStorage(StorageId storage_id);
            Result VerifyContentMetaDatabase(StorageId storage_id);
            Result OpenContentStorage(Out<std::shared_ptr<ContentStorageInterface>> out, StorageId storage_id);
            Result OpenContentMetaDatabase(Out<std::shared_ptr<ContentMetaDatabaseInterface>> out, StorageId storage_id);
            Result CloseContentStorageForcibly(StorageId storage_id);
            Result CloseContentMetaDatabaseForcibly(StorageId storage_id);
            Result CleanupContentMetaDatabase(StorageId storage_id);
            Result ActivateContentStorage(StorageId storage_id);
            Result InactivateContentStorage(StorageId storage_id);
            Result ActivateContentMetaDatabase(StorageId storage_id);
            Result InactivateContentMetaDatabase(StorageId storage_id);

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