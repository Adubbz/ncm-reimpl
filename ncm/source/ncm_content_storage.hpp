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
#include "impl/ncm_placeholder_accessor.hpp"

namespace sts::ncm {

    class ContentStorageInterface : public IServiceObject {
        private:
            char root_path[FS_MAX_PATH-1];
            MakeContentPathFunc make_content_path_func;
            bool disabled;
            impl::PlaceHolderAccessor placeholder_accessor;
            ContentId cached_content_id;
            FILE* content_cache_file_handle;

        private:
            void ClearContentCache();
            unsigned int GetContentDirectoryDepth();
            Result OpenCachedContentFile(ContentId content_id);

            inline void GetContentRootPath(char* out_content_root) {
                /* TODO: Replace with BoundedString? */
                if (snprintf(out_content_root, FS_MAX_PATH-1, "%s%s", this->root_path, "/registered") < 0) {
                    std::abort();
                }
            }

            inline void GetContentPath(char* out_content_root, ContentId content_id) {
                char content_root_path[FS_MAX_PATH] = {0};

                this->GetContentRootPath(content_root_path);
                this->make_content_path_func(out_content_root, content_id, content_root_path);
            }

        private:
            enum class CommandId {
                GeneratePlaceHolderId = 0,
                CreatePlaceHolder = 1,
                DeletePlaceHolder = 2,
                HasPlaceHolder = 3,
                WritePlaceHolder = 4,
                Register = 5,
                Delete = 6,
                Has = 7,
                GetPath = 8,
                GetPlaceHolderPath = 9,
                CleanupAllPlaceHolder = 10,
                ListPlaceHolder = 11,
                GetContentCount = 12,
                ListContentId = 13,
                GetSizeFromContentId = 14,
                DisableForcibly = 15,
                RevertToPlaceHolder = 16,
                SetPlaceHolderSize = 17,
                ReadContentIdFile = 18,
                GetRightsIdFromPlaceHolderId = 19,
                GetRightsIdFromContentId = 20,
                WriteContentForDebug = 21,
                GetFreeSpaceSize = 22,
                GetTotalSpaceSize = 23,
                FlushPlaceHolder = 24,
                GetSizeFromPlaceHolderId = 25,
                RepairInvalidFileAttribute = 26,
                GetRightsIdFromPlaceHolderIdWithCache = 27,
            };

            /* Actual commands. */
            Result GeneratePlaceHolderId(OutPointerWithServerSize<PlaceHolderId, 0x1> out);
            Result CreatePlaceHolder(PlaceHolderId placeholder_id, ContentId content_id, u64 size);
            Result DeletePlaceHolder(PlaceHolderId placeholder_id);
            Result HasPlaceHolder(Out<bool> out, PlaceHolderId placeholder_id);
            Result WritePlaceHolder(PlaceHolderId placeholder_id, u64 offset, InBuffer<u8> data);
            Result Register(PlaceHolderId placeholder_id, ContentId content_id);
            Result Delete(ContentId content_id);
            Result Has(Out<bool> out, ContentId content_id);
            Result GetPath(OutPointerWithClientSize<char> out, ContentId content_id);
            Result GetPlaceHolderPath(OutPointerWithClientSize<char> out, PlaceHolderId placeholder_id);
            Result CleanupAllPlaceHolder();
            Result ListPlaceHolder(Out<u32> out_count, OutBuffer<PlaceHolderId> out_buf);
            Result GetContentCount(Out<u32> out_count);
            Result ListContentId(Out<u32> out_count, OutBuffer<ContentId> out_buf, u32 start_offset);
            Result GetSizeFromContentId(Out<u64> out_size, ContentId content_id);
            Result DisableForcibly();
            Result RevertToPlaceHolder(PlaceHolderId placeholder_id, ContentId old_content_id, ContentId new_content_id);
            Result SetPlaceHolderSize(PlaceHolderId placeholder_id, u64 size);
            Result ReadContentIdFile(OutBuffer<u8> buf, ContentId content_id, u64 offset);
            Result GetRightsIdFromPlaceHolderId(Out<FsRightsId> out_rights_id, Out<u64> out_key_generation, PlaceHolderId placeholder_id);
            Result GetRightsIdFromContentId(Out<FsRightsId> out_rights_id, Out<u64> out_key_generation, ContentId content_id);
            Result WriteContentForDebug(ContentId content_id, u64 offset, InBuffer<u8> data);
            Result GetFreeSpaceSize(Out<u64> out_size);
            Result GetTotalSpaceSize(Out<u64> out_size);
            Result FlushPlaceHolder();
            Result GetSizeFromPlaceHolderId(Out<u64> out, PlaceHolderId placeholder_id);
            Result RepairInvalidFileAttribute();
            Result GetRightsIdFromPlaceHolderIdWithCache(Out<FsRightsId> out_rights_id, Out<u64> out_key_generation, PlaceHolderId placeholder_id, ContentId cache_content_id);

        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                /* 1.0.0- */
                MakeServiceCommandMeta<CommandId::GeneratePlaceHolderId, &ContentStorageInterface::GeneratePlaceHolderId>(),
                MakeServiceCommandMeta<CommandId::CreatePlaceHolder, &ContentStorageInterface::CreatePlaceHolder>(),
                MakeServiceCommandMeta<CommandId::DeletePlaceHolder, &ContentStorageInterface::DeletePlaceHolder>(),
                MakeServiceCommandMeta<CommandId::HasPlaceHolder, &ContentStorageInterface::HasPlaceHolder>(),
                MakeServiceCommandMeta<CommandId::WritePlaceHolder, &ContentStorageInterface::WritePlaceHolder>(),
                MakeServiceCommandMeta<CommandId::Register, &ContentStorageInterface::Register>(),
                MakeServiceCommandMeta<CommandId::Delete, &ContentStorageInterface::Delete>(),
                MakeServiceCommandMeta<CommandId::Has, &ContentStorageInterface::Has>(),
                MakeServiceCommandMeta<CommandId::GetPath, &ContentStorageInterface::GetPath>(),
                MakeServiceCommandMeta<CommandId::GetPlaceHolderPath, &ContentStorageInterface::GetPlaceHolderPath>(),
                MakeServiceCommandMeta<CommandId::CleanupAllPlaceHolder, &ContentStorageInterface::CleanupAllPlaceHolder>(),
                MakeServiceCommandMeta<CommandId::ListPlaceHolder, &ContentStorageInterface::ListPlaceHolder>(),
                MakeServiceCommandMeta<CommandId::GetContentCount, &ContentStorageInterface::GetContentCount>(),
                MakeServiceCommandMeta<CommandId::ListContentId, &ContentStorageInterface::ListContentId>(),
                MakeServiceCommandMeta<CommandId::GetSizeFromContentId, &ContentStorageInterface::GetSizeFromContentId>(),
                MakeServiceCommandMeta<CommandId::DisableForcibly, &ContentStorageInterface::DisableForcibly>(),

                /* 2.0.0- */
                MakeServiceCommandMeta<CommandId::RevertToPlaceHolder, &ContentStorageInterface::RevertToPlaceHolder, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::SetPlaceHolderSize, &ContentStorageInterface::SetPlaceHolderSize, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::ReadContentIdFile, &ContentStorageInterface::ReadContentIdFile, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::GetRightsIdFromPlaceHolderId, &ContentStorageInterface::GetRightsIdFromPlaceHolderId, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::GetRightsIdFromContentId, &ContentStorageInterface::GetRightsIdFromContentId, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::WriteContentForDebug, &ContentStorageInterface::WriteContentForDebug, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::GetFreeSpaceSize, &ContentStorageInterface::GetFreeSpaceSize, FirmwareVersion_200>(),
                MakeServiceCommandMeta<CommandId::GetTotalSpaceSize, &ContentStorageInterface::GetTotalSpaceSize, FirmwareVersion_200>(),

                /* 3.0.0- */
                MakeServiceCommandMeta<CommandId::FlushPlaceHolder, &ContentStorageInterface::FlushPlaceHolder, FirmwareVersion_300>(),

                /* 4.0.0- */
                MakeServiceCommandMeta<CommandId::GetSizeFromPlaceHolderId, &ContentStorageInterface::GetSizeFromPlaceHolderId, FirmwareVersion_400>(),
                MakeServiceCommandMeta<CommandId::RepairInvalidFileAttribute, &ContentStorageInterface::RepairInvalidFileAttribute, FirmwareVersion_400>(),

                /* 8.0.0- */
                MakeServiceCommandMeta<CommandId::GetRightsIdFromPlaceHolderIdWithCache, &ContentStorageInterface::GetRightsIdFromPlaceHolderIdWithCache, FirmwareVersion_800>(),
            };
    };

}