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

#include "ncm_content_storage.hpp"

Result ContentStorageInterface::GeneratePlaceHolderId(OutPointerWithServerSize<NcmNcaId, 0x1> out)
{
    return 0xF601;
}

Result ContentStorageInterface::CreatePlaceHolder(NcmNcaId content_id, NcmNcaId placeholder_id, u64 size)
{
    return 0xF601;
}

Result ContentStorageInterface::DeletePlaceHolder(NcmNcaId placeholder_id)
{
    return 0xF601;
}

Result ContentStorageInterface::HasPlaceHolder(NcmNcaId placeholder_id)
{
    return 0xF601;
}

Result ContentStorageInterface::WritePlaceHolder(NcmNcaId placeholder_id, u64 offset, InBuffer<u8> data)
{
    return 0xF601;
}

Result ContentStorageInterface::Register(NcmNcaId content_id, NcmNcaId placeholder_id)
{
    return 0xF601;
}

Result ContentStorageInterface::Delete(NcmNcaId content_id)
{
    return 0xF601;
}

Result ContentStorageInterface::Has(NcmNcaId content_id)
{
    return 0xF601;
}

Result ContentStorageInterface::GetPath(OutPointerWithClientSize<char> out, NcmNcaId content_id)
{
    return 0xF601;
}

Result ContentStorageInterface::GetPlaceHolderPath(OutPointerWithClientSize<char> out, NcmNcaId placeholder_id)
{
    return 0xF601;
}

Result ContentStorageInterface::CleanupAllPlaceHolder()
{
    return 0xF601;
}

Result ContentStorageInterface::ListPlaceHolder(Out<int> entries_read, OutBuffer<NcmNcaId> out_buf)
{
    return 0xF601;
}

Result ContentStorageInterface::GetContentCount(Out<int> count)
{
    return 0xF601;
}

Result ContentStorageInterface::ListContentId(Out<int> entries_read, OutBuffer<NcmNcaId> out_buf, int start_offset)
{
    return 0xF601;
}

Result ContentStorageInterface::GetSizeFromContentId(Out<u64> size, NcmNcaId content_id)
{
    return 0xF601;
}

Result ContentStorageInterface::DisableForcibly()
{
    return 0xF601;
}

Result ContentStorageInterface::RevertToPlaceHolder(NcmNcaId placeholder_id, NcmNcaId content_id_0, NcmNcaId content_id_1)
{
    return 0xF601;
}

Result ContentStorageInterface::SetPlaceHolderSize(NcmNcaId placeholder_id, u64 size)
{
    return 0xF601;
}

Result ContentStorageInterface::ReadContentIdFile(OutBuffer<u8> buf, NcmNcaId content_id, u64 offset)
{
    return 0xF601;
}

Result ContentStorageInterface::GetRightsIdFromPlaceHolderId(Out<NcmRightsId> out, NcmNcaId placeholder_id)
{
    return 0xF601;
}

Result ContentStorageInterface::GetRightsIdFromContentId(Out<NcmRightsId> out, NcmNcaId content_id)
{
    return 0xF601;
}

Result ContentStorageInterface::WriteContentForDebug(NcmNcaId content_id, u64 offset, InBuffer<u8> data)
{
    return 0xF601;
}

Result ContentStorageInterface::GetFreeSpaceSize(Out<u64> out)
{
    return 0xF601;
}

Result ContentStorageInterface::GetTotalSpaceSize(Out<u64> out)
{
    return 0xF601;
}

Result ContentStorageInterface::FlushPlaceHolder()
{
    return 0xF601;
}

Result ContentStorageInterface::GetSizeFromPlaceHolderId(Out<u64> out, NcmNcaId placeholder_id)
{
    return 0xF601;
}

Result ContentStorageInterface::RepairInvalidFileAttribute()
{
    return 0xF601;
}