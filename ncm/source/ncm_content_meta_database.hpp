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

#include "ncm_types.hpp"

namespace sts::ncm {

    class ContentMetaDatabaseInterface : public IServiceObject {
        private:
            enum class CommandId {
                Set = 0,
                Get = 1,
                Remove = 2,
                GetContentIdByType = 3,
                ListContentInfo = 4,
                List = 5,
                GetLatestContentMetaKey = 6,
                ListApplication = 7,
                Has = 8,
                HasAll = 9,
                GetSize = 10,
                GetRequiredSystemVersion = 11,
                GetPatchId = 12,
                DisableForcibly = 13,
                LookupOrphanContent = 14,
                Commit = 15,
                HasContent = 16,
                ListContentMetaInfo = 17,
                GetAttributes = 18,
                GetRequiredApplicationVersion = 19,
                GetContentIdByTypeAndIdOffset = 20,
            };
        
            /* Actual commands */
            Result Set(ContentMetaKey key, InBuffer<u8> value);
            Result Get(Out<u64> out_size_read, ContentMetaKey key, OutBuffer<u8> out_val_buf);
            Result Remove(ContentMetaKey key);
            Result GetContentIdByType(Out<ContentId> out_content_id, ContentMetaKey key, ContentType type);
            Result ListContentInfo(Out<u32> out_entries_read, OutBuffer<ContentInfo> out_info, ContentMetaKey key, u32 start_index);
            Result List(Out<u32> out_entries_total, Out<u32> out_entries_written, OutBuffer<ContentInfo> out_info, ContentMetaType meta_type, TitleId application_title_id, TitleId title_id_min, TitleId title_id_max, ContentMetaAttribute attributes);
            Result GetLatestContentMetaKey(Out<ContentMetaKey> out_key, TitleId title_id);
            Result ListApplication(Out<u32> out_entries_total, Out<u32> out_entries_written, OutBuffer<ApplicationContentMetaKey> out_keys, ContentMetaType meta_type);
            Result Has(Out<bool> out, ContentMetaKey key);
            Result HasAll(Out<bool> out, InBuffer<ContentMetaKey> keys);
            Result GetSize(Out<u64> out_size, ContentMetaKey key);
            Result GetRequiredSystemVersion(Out<u32> out_version, ContentMetaKey key);
            Result GetPatchId(Out<TitleId> out_patch_id, ContentMetaKey key);
            Result DisableForcibly();
            Result LookupOrphanContent(OutBuffer<bool> out_orphaned, InBuffer<ContentId> content_ids);
            Result Commit();
            Result HasContent(Out<bool> out, ContentMetaKey key, ContentId content_id);
            Result ListContentMetaInfo(Out<u32> out_entries_written, OutBuffer<ContentMetaInfo> out_info, ContentMetaKey key, u32 start_index);
            Result GetAttributes(Out<ContentMetaAttribute> out_attributes, ContentMetaKey key);
            Result GetRequiredApplicationVersion(Out<u32> out_version, ContentMetaKey key);
            Result GetContentIdByTypeAndIdOffset(Out<ContentId> out_content_id, ContentMetaKey key, ContentType type, u8 id_offset);

        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                /* 1.0.0- */
                MakeServiceCommandMeta<CommandId::Set, &ContentStorageInterface::Set>(),
                MakeServiceCommandMeta<CommandId::Get, &ContentStorageInterface::Get>(),
                MakeServiceCommandMeta<CommandId::Remove, &ContentStorageInterface::Remove>(),
                MakeServiceCommandMeta<CommandId::GetContentIdByType, &ContentStorageInterface::GetContentIdByType>(),
                MakeServiceCommandMeta<CommandId::ListContentInfo, &ContentStorageInterface::ListContentInfo>(),
                MakeServiceCommandMeta<CommandId::List, &ContentStorageInterface::List>(),
                MakeServiceCommandMeta<CommandId::GetLatestContentMetaKey, &ContentStorageInterface::GetLatestContentMetaKey>(),
                MakeServiceCommandMeta<CommandId::ListApplication, &ContentStorageInterface::ListApplication>(),
                MakeServiceCommandMeta<CommandId::Has, &ContentStorageInterface::Has>(),
                MakeServiceCommandMeta<CommandId::HasAll, &ContentStorageInterface::HasAll>(),
                MakeServiceCommandMeta<CommandId::GetSize, &ContentStorageInterface::GetSize>(),
                MakeServiceCommandMeta<CommandId::GetRequiredSystemVersion, &ContentStorageInterface::GetRequiredSystemVersion>(),
                MakeServiceCommandMeta<CommandId::GetPatchId, &ContentStorageInterface::GetPatchId>(),
                MakeServiceCommandMeta<CommandId::DisableForcibly, &ContentStorageInterface::DisableForcibly>(),
                MakeServiceCommandMeta<CommandId::LookupOrphanContent, &ContentStorageInterface::LookupOrphanContent>(),
                MakeServiceCommandMeta<CommandId::Commit, &ContentStorageInterface::Commit>(),
                MakeServiceCommandMeta<CommandId::HasContent, &ContentStorageInterface::HasContent>(),
                MakeServiceCommandMeta<CommandId::ListContentMetaInfo, &ContentStorageInterface::ListContentMetaInfo>(),
                MakeServiceCommandMeta<CommandId::GetAttributes, &ContentStorageInterface::GetAttributes>(),

                /* 2.0.0- */
                MakeServiceCommandMeta<CommandId::GetRequiredApplicationVersion, &ContentStorageInterface::GetRequiredApplicationVersion, FirmwareVersion_200>(),

                /* 5.0.0- */
                MakeServiceCommandMeta<CommandId::GetContentIdByTypeAndIdOffset, &ContentStorageInterface::GetContentIdByTypeAndIdOffset, FirmwareVersion_500>(),
            };
    };

}