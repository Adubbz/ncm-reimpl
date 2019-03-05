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

enum CsCmd : u32
{
    Cs_Cmd_GeneratePlaceHolderId = 0,
    Cs_Cmd_CreatePlaceHolder = 1,
    Cs_Cmd_DeletePlaceHolder = 2,
    Cs_Cmd_HasPlaceHolder = 3,
    Cs_Cmd_WritePlaceHolder = 4,
    Cs_Cmd_Register = 5,
    Cs_Cmd_Delete = 6,
    Cs_Cmd_Has = 7,
    Cs_Cmd_GetPath = 8,
    Cs_Cmd_GetPlaceHolderPath = 9,
    Cs_Cmd_CleanupAllPlaceHolder = 10,
    Cs_Cmd_ListPlaceHolder = 11,
    Cs_Cmd_GetContentCount = 12,
    Cs_Cmd_ListContentId = 13,
    Cs_Cmd_GetSizeFromContentId = 14,
    Cs_Cmd_DisableForcibly = 15,
    Cs_Cmd_RevertToPlaceHolder = 16,
    Cs_Cmd_SetPlaceHolderSize = 17,
    Cs_Cmd_ReadContentIdFile = 18,
    Cs_Cmd_GetRightsIdFromPlaceHolderId = 19,
    Cs_Cmd_GetRightsIdFromContentId = 20,
    Cs_Cmd_WriteContentForDebug = 21,
    Cs_Cmd_GetFreeSpaceSize = 22,
    Cs_Cmd_GetTotalSpaceSize = 23,
    Cs_Cmd_FlushPlaceHolder = 24,
    Cs_Cmd_GetSizeFromPlaceHolderId = 25,
    Cs_Cmd_RepairInvalidFileAttribute = 26,
};

class ContentStorageInterface : public IServiceObject
{
    private:
        /* Actual commands. */
        Result GeneratePlaceHolderId(OutPointerWithServerSize<NcmNcaId, 0x1> out);
        Result CreatePlaceHolder(NcmNcaId content_id, NcmNcaId placeholder_id, u64 size);
        Result DeletePlaceHolder(NcmNcaId placeholder_id);
        Result HasPlaceHolder(NcmNcaId placeholder_id);
        Result WritePlaceHolder(NcmNcaId placeholder_id, u64 offset, InBuffer<u8> data);
        Result Register(NcmNcaId content_id, NcmNcaId placeholder_id);
        Result Delete(NcmNcaId content_id);
        Result Has(Out<bool> out, NcmNcaId content_id);
        Result GetPath(OutPointerWithClientSize<char> out, NcmNcaId content_id);
        Result GetPlaceHolderPath(OutPointerWithClientSize<char> out, NcmNcaId placeholder_id);
        Result CleanupAllPlaceHolder();
        Result ListPlaceHolder(Out<int> entries_read, OutBuffer<NcmNcaId> out_buf);
        Result GetContentCount(Out<int> count);
        Result ListContentId(Out<int> entries_read, OutBuffer<NcmNcaId> out_buf, int start_offset);
        Result GetSizeFromContentId(Out<u64> size, NcmNcaId content_id);
        Result DisableForcibly();
        Result RevertToPlaceHolder(NcmNcaId placeholder_id, NcmNcaId content_id_0, NcmNcaId content_id_1);
        Result SetPlaceHolderSize(NcmNcaId placeholder_id, u64 size);
        Result ReadContentIdFile(OutBuffer<u8> buf, NcmNcaId content_id, u64 offset);
        Result GetRightsIdFromPlaceHolderId(Out<NcmRightsId> out, NcmNcaId placeholder_id);
        Result GetRightsIdFromContentId(Out<NcmRightsId> out, NcmNcaId content_id);
        Result WriteContentForDebug(NcmNcaId content_id, u64 offset, InBuffer<u8> data);
        Result GetFreeSpaceSize(Out<u64> out);
        Result GetTotalSpaceSize(Out<u64> out);
        Result FlushPlaceHolder();
        Result GetSizeFromPlaceHolderId(Out<u64> out, NcmNcaId placeholder_id);
        Result RepairInvalidFileAttribute();

    public:
        DEFINE_SERVICE_DISPATCH_TABLE
        {
            /* 1.0.0- */
            MakeServiceCommandMeta<Cs_Cmd_GeneratePlaceHolderId, &ContentStorageInterface::GeneratePlaceHolderId>(),
            MakeServiceCommandMeta<Cs_Cmd_CreatePlaceHolder, &ContentStorageInterface::CreatePlaceHolder>(),
            MakeServiceCommandMeta<Cs_Cmd_DeletePlaceHolder, &ContentStorageInterface::DeletePlaceHolder>(),
            MakeServiceCommandMeta<Cs_Cmd_HasPlaceHolder, &ContentStorageInterface::HasPlaceHolder>(),
            MakeServiceCommandMeta<Cs_Cmd_WritePlaceHolder, &ContentStorageInterface::WritePlaceHolder>(),
            MakeServiceCommandMeta<Cs_Cmd_Register, &ContentStorageInterface::Register>(),
            MakeServiceCommandMeta<Cs_Cmd_Delete, &ContentStorageInterface::Delete>(),
            MakeServiceCommandMeta<Cs_Cmd_Has, &ContentStorageInterface::Has>(),
            MakeServiceCommandMeta<Cs_Cmd_GetPath, &ContentStorageInterface::GetPath>(),
            MakeServiceCommandMeta<Cs_Cmd_GetPlaceHolderPath, &ContentStorageInterface::GetPlaceHolderPath>(),
            MakeServiceCommandMeta<Cs_Cmd_CleanupAllPlaceHolder, &ContentStorageInterface::CleanupAllPlaceHolder>(),
            MakeServiceCommandMeta<Cs_Cmd_ListPlaceHolder, &ContentStorageInterface::ListPlaceHolder>(),
            MakeServiceCommandMeta<Cs_Cmd_GetContentCount, &ContentStorageInterface::GetContentCount>(),
            MakeServiceCommandMeta<Cs_Cmd_ListContentId, &ContentStorageInterface::ListContentId>(),
            MakeServiceCommandMeta<Cs_Cmd_GetSizeFromContentId, &ContentStorageInterface::GetSizeFromContentId>(),
            MakeServiceCommandMeta<Cs_Cmd_DisableForcibly, &ContentStorageInterface::DisableForcibly>(),

            /* 2.0.0- */
            MakeServiceCommandMeta<Cs_Cmd_RevertToPlaceHolder, &ContentStorageInterface::RevertToPlaceHolder, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_SetPlaceHolderSize, &ContentStorageInterface::SetPlaceHolderSize, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_ReadContentIdFile, &ContentStorageInterface::ReadContentIdFile, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_GetRightsIdFromPlaceHolderId, &ContentStorageInterface::GetRightsIdFromPlaceHolderId, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_GetRightsIdFromContentId, &ContentStorageInterface::GetRightsIdFromContentId, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_WriteContentForDebug, &ContentStorageInterface::WriteContentForDebug, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_GetFreeSpaceSize, &ContentStorageInterface::GetFreeSpaceSize, FirmwareVersion_200>(),
            MakeServiceCommandMeta<Cs_Cmd_GetTotalSpaceSize, &ContentStorageInterface::GetTotalSpaceSize, FirmwareVersion_200>(),

            /* 3.0.0- */
            MakeServiceCommandMeta<Cs_Cmd_FlushPlaceHolder, &ContentStorageInterface::FlushPlaceHolder, FirmwareVersion_300>(),

            /* 4.0.0- */
            MakeServiceCommandMeta<Cs_Cmd_GetSizeFromPlaceHolderId, &ContentStorageInterface::GetSizeFromPlaceHolderId, FirmwareVersion_400>(),
            MakeServiceCommandMeta<Cs_Cmd_RepairInvalidFileAttribute, &ContentStorageInterface::RepairInvalidFileAttribute, FirmwareVersion_400>(),
        };
};