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
#include "fs_utils.hpp"

Result ContentStorageInterface::GeneratePlaceHolderId(OutPointerWithServerSize<PlaceHolderId, 0x1> out)
{
    if (this->disabled)
        return ResultNcmInvalidContentStorage;

    StratosphereRandomUtils::GetRandomBytes(out.pointer, sizeof(NcmNcaId));
    return ResultSuccess;
}

Result ContentStorageInterface::CreatePlaceHolder(PlaceHolderId placeholder_id, ContentId content_id, u64 size)
{
    Result rc = ResultSuccess;

    if (!this->disabled) {
        char content_root_path[FS_MAX_PATH] = {0};
        char content_path[FS_MAX_PATH] = {0};

        /* TODO: Replace with BoundedString? */
        snprintf(content_root_path, FS_MAX_PATH, "%s%s", this->placeholder_accessor.root_path, "/registered");
        this->make_content_path_func(content_path, content_id, content_root_path);
    
        if (R_FAILED(rc = FsUtils::EnsureParentDirectoryRecursively(content_path))) {
            return rc;
        }

        if (R_FAILED(rc = ContentUtils::CreatePlaceHolderFile(&this->placeholder_accessor, placeholder_id, size))) {
            return rc;
        }
    }
    else {
        return ResultNcmInvalidContentStorage;
    }

    return ResultSuccess;
}

Result ContentStorageInterface::DeletePlaceHolder(PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::HasPlaceHolder(PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::WritePlaceHolder(PlaceHolderId placeholder_id, u64 offset, InBuffer<u8> data)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::Register(ContentId content_id, PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::Delete(ContentId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::Has(Out<bool> out, ContentId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetPath(OutPointerWithClientSize<char> out, ContentId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetPlaceHolderPath(OutPointerWithClientSize<char> out, PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::CleanupAllPlaceHolder()
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::ListPlaceHolder(Out<int> entries_read, OutBuffer<PlaceHolderId> out_buf)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetContentCount(Out<int> count)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::ListContentId(Out<int> entries_read, OutBuffer<ContentId> out_buf, int start_offset)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetSizeFromContentId(Out<u64> size, ContentId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::DisableForcibly()
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::RevertToPlaceHolder(PlaceHolderId placeholder_id, ContentId content_id_0, ContentId content_id_1)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::SetPlaceHolderSize(PlaceHolderId placeholder_id, u64 size)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::ReadContentIdFile(OutBuffer<u8> buf, ContentId content_id, u64 offset)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetRightsIdFromPlaceHolderId(Out<NcmRightsId> out, PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetRightsIdFromContentId(Out<NcmRightsId> out, ContentId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::WriteContentForDebug(ContentId content_id, u64 offset, InBuffer<u8> data)
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

Result ContentStorageInterface::GetSizeFromPlaceHolderId(Out<u64> out, PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::RepairInvalidFileAttribute()
{
    return ResultKernelConnectionClosed;
}