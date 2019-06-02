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
#include "ncm_utils.hpp"
#include "ncm_path_utils.hpp"

void ContentStorageInterface::MakeContentPathUnlayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%s", root, content_name) < 0) {
        std::abort();
    }
}

void ContentStorageInterface::MakeContentPathHashByteLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_byte = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_byte = hash[0];
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_byte, content_name) < 0) {
        std::abort();
    }
}

void ContentStorageInterface::MakeContentPath10BitLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_bytes = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_bytes = (*((u16*)hash) & 0xff00) >> 6;
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%s", root, hash_bytes, content_name) < 0) {
        std::abort();
    }
}

void ContentStorageInterface::MakeContentPathDualLayered(char* path_out, ContentId content_id, const char* root) {
    char content_name[FS_MAX_PATH] = {0};
    u8 hash[0x20] = {0};
    u32 hash_lower = 0;
    u32 hash_upper = 0;

    sha256CalculateHash(hash, content_id.uuid, sizeof(ContentId));
    hash_lower = (*((u16*)hash) >> 4) & 0x3f;
    hash_upper = (*((u16*)hash) & 0xff00) >> 10;
    PathUtils::GetContentFileName(content_name, content_id);
    if (snprintf(path_out, FS_MAX_PATH-1, "%s/%08X/%08X/%s", root, hash_upper, hash_lower, content_name) < 0) {
        std::abort();
    }
}

void ContentStorageInterface::ClearContentCache() {
    if (memcmp(this->cached_content_id.uuid, InvalidUuid.uuid, sizeof(ContentId)) != 0) {
        fclose(this->content_cache_file_handle);
        this->cached_content_id = InvalidUuid;
    }
}

unsigned int ContentStorageInterface::GetContentDirectoryDepth() {
    if (this->make_content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPathUnlayered)) {
        return 1;
    }
    else if (this->make_content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPathHashByteLayered)) {
        return 2;
    }
    else if (this->make_content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPath10BitLayered)) {
        return 2;
    }
    else if (this->make_content_path_func == reinterpret_cast<MakeContentPathFunc>(MakeContentPathDualLayered)) {
        return 3;
    }

    std::abort();
}

Result ContentStorageInterface::OpenCachedContentFile(ContentId content_id) {
    if (memcmp(this->cached_content_id.uuid, content_id.uuid, sizeof(ContentId)) == 0) {
        return ResultSuccess;
    }

    this->ClearContentCache();
    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);
    errno = 0;
    this->content_cache_file_handle = fopen(content_path, "rb");

    if (this->content_cache_file_handle == NULL || errno != 0) {
        R_TRY_CATCH(fsdevGetLastResult()) {
            R_CATCH(ResultFsPathNotFound) {
                return ResultNcmContentNotFound;
            }
        } R_END_TRY_CATCH;
    }

    this->cached_content_id = content_id;
    return ResultSuccess;
}

Result ContentStorageInterface::GeneratePlaceHolderId(OutPointerWithServerSize<PlaceHolderId, 0x1> out) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    StratosphereRandomUtils::GetRandomBytes(out.pointer, sizeof(NcmNcaId));
    return ResultSuccess;
}

Result ContentStorageInterface::CreatePlaceHolder(PlaceHolderId placeholder_id, ContentId content_id, u64 size) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);

    R_TRY(FsUtils::EnsureParentDirectoryRecursively(content_path));
    R_TRY(this->placeholder_accessor.Create(placeholder_id, size));

    return ResultSuccess;
}

Result ContentStorageInterface::DeletePlaceHolder(PlaceHolderId placeholder_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    return this->placeholder_accessor.Delete(placeholder_id);
}

Result ContentStorageInterface::HasPlaceHolder(Out<bool> out, PlaceHolderId placeholder_id) {
    if (this->disabled)
        return ResultNcmInvalidContentStorage;

    char placeholder_path[FS_MAX_PATH] = {0};
    this->placeholder_accessor.GetPlaceHolderPath(placeholder_path, placeholder_id);

    errno = 0;
    out.SetValue(false);

    if (access(placeholder_path, F_OK) != -1) {
        out.SetValue(true);
    }
    else if (errno != 0 && errno != ENOENT && errno != ENOTDIR) {
        return fsdevGetLastResult();
    }

    return ResultSuccess;
}

Result ContentStorageInterface::WritePlaceHolder(PlaceHolderId placeholder_id, u64 offset, InBuffer<u8> data) {
    /* Offset is too large */
    if (offset >> 0x3f != 0) {
        return ResultNcmInvalidOffset;
    }

    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    FILE* f = nullptr;

    R_TRY_CATCH(this->placeholder_accessor.Open(&f, placeholder_id)) {
        R_CATCH(ResultFsPathNotFound) {
            return ResultNcmPlaceHolderNotFound;
        }
    } R_END_TRY_CATCH;

    errno = 0;
    fseek(f, offset, SEEK_SET);
    fwrite(data.buffer, sizeof(u8), data.num_elements, f);

    if (this->placeholder_accessor.delay_flush ^ 1) {
        fsync(fileno(f));
    }

    this->placeholder_accessor.FlushCache(f, placeholder_id);

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    return ResultSuccess;
}

Result ContentStorageInterface::Register(PlaceHolderId placeholder_id, ContentId content_id) {
    this->ClearContentCache();
    
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char placeholder_path[FS_MAX_PATH] = {0};
    char content_path[FS_MAX_PATH] = {0};

    this->placeholder_accessor.GetPlaceHolderPathUncached(placeholder_path, placeholder_id);
    this->GetContentPath(content_path, content_id);

    errno = 0;
    rename(placeholder_path, content_path);

    if (errno != 0) {
        R_TRY_CATCH(fsdevGetLastResult()) {
            R_CATCH(ResultFsPathNotFound) {
                return ResultNcmPlaceHolderNotFound;
            }
            R_CATCH(ResultFsPathAlreadyExists) {
                return ResultNcmContentAlreadyExists;
            }
        } R_END_TRY_CATCH;
    }

    return ResultSuccess;
}

Result ContentStorageInterface::Delete(ContentId content_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    this->ClearContentCache();

    char content_path[FS_MAX_PATH] = {0};

    this->GetContentPath(content_path, content_id);

    R_TRY_CATCH(fsdevDeleteDirectoryRecursively(content_path)) {
        R_CATCH(ResultFsPathNotFound) {
            return ResultNcmContentNotFound;
        }
    } R_END_TRY_CATCH;

    return ResultSuccess;
}

Result ContentStorageInterface::Has(Out<bool> out, ContentId content_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);

    errno = 0;
    out.SetValue(false);

    if (access(content_path, F_OK) != -1) {
        out.SetValue(true);
    }
    else if (errno != 0 && errno != ENOENT && errno != ENOTDIR) {
        return fsdevGetLastResult();
    }

    return ResultSuccess;
}

Result ContentStorageInterface::GetPath(OutPointerWithClientSize<char> out, ContentId content_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);
    memcpy(out.pointer, content_path, FS_MAX_PATH-1);

    return ResultSuccess;
}

Result ContentStorageInterface::GetPlaceHolderPath(OutPointerWithClientSize<char> out, PlaceHolderId placeholder_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char placeholder_path[FS_MAX_PATH] = {0};
    this->placeholder_accessor.GetPlaceHolderPathUncached(placeholder_path, placeholder_id);
    memcpy(out.pointer, placeholder_path, FS_MAX_PATH-1);

    return ResultSuccess;
}

Result ContentStorageInterface::CleanupAllPlaceHolder() {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char placeholder_root_path[FS_MAX_PATH] = {0};
    this->placeholder_accessor.ClearAllCaches();
    this->placeholder_accessor.GetPlaceHolderRootPath(placeholder_root_path);

    R_TRY(fsdevDeleteDirectoryRecursively(placeholder_root_path));

    return ResultSuccess;
}

Result ContentStorageInterface::ListPlaceHolder(Out<u32> out_count, OutBuffer<PlaceHolderId> out_buf) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char placeholder_root_path[FS_MAX_PATH] = {0};
    this->placeholder_accessor.GetPlaceHolderRootPath(placeholder_root_path);
    unsigned int dir_depth = this->placeholder_accessor.GetDirectoryDepth();
    size_t entry_count = 0;

    R_TRY(FsUtils::TraverseDirectory(placeholder_root_path, dir_depth, [&](bool* should_continue, bool* should_retry_dir_read, const char* current_path, struct dirent* dir_entry) {
        *should_continue = true;
        *should_retry_dir_read = false;
        
        if (dir_entry->d_type == DT_REG) {
            if (entry_count > out_buf.num_elements) {
                return ResultNcmBufferInsufficient;
            }
            
            PlaceHolderId cur_entry_placeholder_id = {0};
            R_TRY(NcmUtils::GetPlaceHolderIdFromDirEntry(&cur_entry_placeholder_id, dir_entry));
            out_buf[entry_count++] = cur_entry_placeholder_id;
        }
        
        return ResultSuccess;
    }));

    out_count.SetValue(static_cast<u32>(entry_count));
    return ResultSuccess;
}

Result ContentStorageInterface::GetContentCount(Out<u32> out_count) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_root_path[FS_MAX_PATH] = {0};
    this->GetContentRootPath(content_root_path);
    unsigned int dir_depth = this->GetContentDirectoryDepth();
    u32 content_count = 0;

    R_TRY(FsUtils::TraverseDirectory(content_root_path, dir_depth, [&](bool* should_continue, bool* should_retry_dir_read, const char* current_path, struct dirent* dir_entry) {
        *should_continue = true;
        *should_retry_dir_read = false;

        if (dir_entry->d_type == DT_REG) {
            content_count++;
        }

        return ResultSuccess;
    }));

    out_count.SetValue(content_count);
    return ResultSuccess;
}

Result ContentStorageInterface::ListContentId(Out<u32> out_count, OutBuffer<ContentId> out_buf, u32 start_offset) {    
    if (start_offset >> 0x1f != 0) {
        return ResultNcmInvalidOffset;
    }

    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_root_path[FS_MAX_PATH] = {0};
    this->GetContentRootPath(content_root_path);
    unsigned int dir_depth = this->GetContentDirectoryDepth();
    size_t entry_count = 0;

    R_TRY(FsUtils::TraverseDirectory(content_root_path, dir_depth, [&](bool* should_continue,  bool* should_retry_dir_read, const char* current_path, struct dirent* dir_entry) {
        *should_retry_dir_read = false;
        *should_continue = true;

        if (dir_entry->d_type == DT_REG) {
            /* Skip entries until we reach the start offset. */
            if (start_offset > 0) {
                start_offset--;
                return ResultSuccess;
            }

            /* We don't necessarily expect to be able to completely fill the output buffer. */
            if (entry_count > out_buf.num_elements) {
                *should_continue = false;
                return ResultSuccess;
            }

            size_t name_len = strlen(dir_entry->d_name);
            std::optional<ContentId> content_id = std::nullopt;
            NcmUtils::GetContentIdFromString(dir_entry->d_name, name_len, &content_id);

            /* Skip to the next entry if the id was invalid. */
            if (!content_id) {
                return ResultSuccess;
            }

            out_buf[entry_count++] = *content_id;
        }

        return ResultSuccess;
    }));

    out_count.SetValue(static_cast<u32>(entry_count));
    return ResultSuccess;
}

Result ContentStorageInterface::GetSizeFromContentId(Out<u64> out_size, ContentId content_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);
    struct stat st;

    errno = 0;
    stat(content_path, &st);

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    out_size.SetValue(st.st_size);
    return ResultSuccess;
}

Result ContentStorageInterface::DisableForcibly() {
    this->disabled = true;
    this->ClearContentCache();
    this->placeholder_accessor.ClearAllCaches();
    return ResultSuccess;
}

Result ContentStorageInterface::RevertToPlaceHolder(PlaceHolderId placeholder_id, ContentId old_content_id, ContentId new_content_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }
    
    char old_content_path[FS_MAX_PATH] = {0};
    char new_content_path[FS_MAX_PATH] = {0};
    char placeholder_path[FS_MAX_PATH] = {0};

    this->ClearContentCache();

    /* Ensure the new content path is ready. */
    this->GetContentPath(new_content_path, new_content_id);
    R_TRY(FsUtils::EnsureParentDirectoryRecursively(new_content_path));

    R_TRY(this->placeholder_accessor.EnsureRecursively(placeholder_id));
    this->placeholder_accessor.GetPlaceHolderPathUncached(placeholder_path, placeholder_id);
    errno = 0;
    rename(old_content_path, placeholder_path);

    if (errno != 0) {
        R_TRY_CATCH(fsdevGetLastResult()) {
            R_CATCH(ResultFsPathNotFound) {
                return ResultNcmPlaceHolderNotFound;
            }
            R_CATCH(ResultFsPathAlreadyExists) {
                return ResultNcmPlaceHolderAlreadyExists;
            }
        } R_END_TRY_CATCH;
    } 

    return ResultSuccess;
}

Result ContentStorageInterface::SetPlaceHolderSize(PlaceHolderId placeholder_id, u64 size) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    R_TRY(this->placeholder_accessor.SetSize(placeholder_id, size));
    return ResultSuccess;
}

Result ContentStorageInterface::ReadContentIdFile(OutBuffer<u8> buf, ContentId content_id, u64 offset) {
    /* Offset is too large */
    if (offset >> 0x3f != 0) {
        return ResultNcmInvalidOffset;
    }

    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);
    R_TRY(this->OpenCachedContentFile(content_id));

    errno = 0;    
    fseek(this->content_cache_file_handle, offset, SEEK_SET);
    fread(buf.buffer, buf.num_elements, 1, this->content_cache_file_handle);

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    return ResultSuccess;
}

Result ContentStorageInterface::GetRightsIdFromPlaceHolderId(Out<RightsId> out, PlaceHolderId placeholder_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::GetRightsIdFromContentId(Out<RightsId> out, ContentId content_id)
{
    return ResultKernelConnectionClosed;
}

Result ContentStorageInterface::WriteContentForDebug(ContentId content_id, u64 offset, InBuffer<u8> data) {
    FILE* f = nullptr;

    ON_SCOPE_EXIT {
        fclose(f);
    };
    
    /* Offset is too large */
    if (offset >> 0x3f != 0) {
        return ResultNcmInvalidOffset;
    }

    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    /* N aborts here if splIsDevelopment returns false. We're not going to. */

    this->ClearContentCache();

    char content_path[FS_MAX_PATH] = {0};
    this->GetContentPath(content_path, content_id);

    errno = 0;
    f = fopen(content_path, "w+b");

    if (f == NULL || errno != 0) {
        return fsdevGetLastResult();
    }

    fseek(f, offset, SEEK_SET);
    fwrite(data.buffer, sizeof(u8), data.num_elements, f);
    fsync(fileno(f));

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    return ResultSuccess;
}

Result ContentStorageInterface::GetFreeSpaceSize(Out<u64> out_size) {
    struct statvfs st = {0};
    errno = 0;
    statvfs(this->root_path, &st);

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    out_size.SetValue(st.f_bfree);
    return ResultSuccess;
}

Result ContentStorageInterface::GetTotalSpaceSize(Out<u64> out_size) {
    struct statvfs st = {0};
    errno = 0;
    statvfs(this->root_path, &st);

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    out_size.SetValue(st.f_blocks);
    return ResultSuccess;
}

Result ContentStorageInterface::FlushPlaceHolder() {
    this->placeholder_accessor.ClearAllCaches();
    return ResultSuccess;
}

Result ContentStorageInterface::GetSizeFromPlaceHolderId(Out<u64> out_size, PlaceHolderId placeholder_id) {
    if (this->disabled) {
        return ResultNcmInvalidContentStorage;
    }

    bool found_in_cache = false;
    size_t size = 0;

    R_TRY(this->placeholder_accessor.GetSize(&found_in_cache, &size, placeholder_id));

    if (found_in_cache) {
        out_size.SetValue(size);
        return ResultSuccess;
    }

    char placeholder_path[FS_MAX_PATH] = {0};
    struct stat st;

    this->placeholder_accessor.GetPlaceHolderPathUncached(placeholder_path, placeholder_id);
    errno = 0;
    stat(placeholder_path, &st);

    if (errno != 0) {
        return fsdevGetLastResult();
    }

    out_size.SetValue(st.st_size);
    return ResultSuccess;
}

Result ContentStorageInterface::RepairInvalidFileAttribute() {
    char content_root_path[FS_MAX_PATH] = {0};
    this->GetContentRootPath(content_root_path);
    unsigned int dir_depth = this->GetContentDirectoryDepth();
    auto fix_file_attributes = [&](bool* should_continue, bool* should_retry_dir_read, const char* current_path, struct dirent* dir_entry) {
        *should_retry_dir_read = false;
        *should_continue = true;

        if (dir_entry->d_type == DT_DIR) {
            if (PathUtils::IsNcaPath(current_path)) {
                if (R_SUCCEEDED(fsdevSetArchiveBit(current_path))) {
                    *should_retry_dir_read = true;
                }
            }
        }

        return ResultSuccess;
    };

    R_TRY(FsUtils::TraverseDirectory(content_root_path, dir_depth, fix_file_attributes));

    char placeholder_root_path[FS_MAX_PATH] = {0};
    this->placeholder_accessor.ClearAllCaches();
    this->placeholder_accessor.GetPlaceHolderRootPath(placeholder_root_path);
    dir_depth = this->placeholder_accessor.GetDirectoryDepth();

    R_TRY(FsUtils::TraverseDirectory(placeholder_root_path, dir_depth, fix_file_attributes));

    return ResultSuccess;
}

Result ContentStorageInterface::GetRightsIdFromPlaceHolderIdWithCache(Out<RightsId> out, PlaceHolderId placeholder_id, ContentId content_id) {
    return ResultKernelConnectionClosed;
}