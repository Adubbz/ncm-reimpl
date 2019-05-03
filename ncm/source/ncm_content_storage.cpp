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
    if (this->invalidated)
        return ResultNcmInvalidContentStorage;

    StratosphereRandomUtils::GetRandomBytes(out.pointer, sizeof(NcmNcaId));
    return ResultSuccess;
}

Result ContentStorageInterface::CreatePlaceHolder(NcmNcaId content_id, NcmNcaId placeholder_id, u64 size)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::DeletePlaceHolder(NcmNcaId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::HasPlaceHolder(NcmNcaId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::WritePlaceHolder(NcmNcaId placeholder_id, u64 offset, InBuffer<u8> data)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::Register(NcmNcaId content_id, NcmNcaId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::Delete(NcmNcaId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::Has(Out<bool> out, NcmNcaId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetPath(OutPointerWithClientSize<char> out, NcmNcaId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetPlaceHolderPath(OutPointerWithClientSize<char> out, NcmNcaId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::CleanupAllPlaceHolder()
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::ListPlaceHolder(Out<int> entries_read, OutBuffer<NcmNcaId> out_buf)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetContentCount(Out<int> count)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::ListContentId(Out<int> entries_read, OutBuffer<NcmNcaId> out_buf, int start_offset)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetSizeFromContentId(Out<u64> size, NcmNcaId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::DisableForcibly()
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::RevertToPlaceHolder(NcmNcaId placeholder_id, NcmNcaId content_id_0, NcmNcaId content_id_1)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::SetPlaceHolderSize(NcmNcaId placeholder_id, u64 size)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::ReadContentIdFile(OutBuffer<u8> buf, NcmNcaId content_id, u64 offset)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetRightsIdFromPlaceHolderId(Out<NcmRightsId> out, NcmNcaId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetRightsIdFromContentId(Out<NcmRightsId> out, NcmNcaId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::WriteContentForDebug(NcmNcaId content_id, u64 offset, InBuffer<u8> data)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetFreeSpaceSize(Out<u64> out)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetTotalSpaceSize(Out<u64> out)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::FlushPlaceHolder()
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetSizeFromPlaceHolderId(Out<u64> out, NcmNcaId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::RepairInvalidFileAttribute()
{
    return ResultKernelConnectionClosed;
}