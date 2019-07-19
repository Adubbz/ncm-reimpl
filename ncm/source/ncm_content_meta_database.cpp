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

#include "ncm_content_meta_database.hpp"
#include "ncm_utils.hpp"

namespace sts::ncm {

    namespace {

        struct InstallContentMetaHeader {
            u16 extended_header_size;
            u16 content_count;
            u16 content_meta_count;
            u16 padding;
        };

        static_assert(sizeof(InstallContentMetaHeader) == 0x8, "InstallContentMetaHeader definition!");

        Result GetContentMetaSize(size_t *out, const ContentMetaKey &key, const kvdb::MemoryKeyValueStore<ContentMetaKey> *kvs) {
            R_TRY_CATCH(kvs->GetValueSize(out, key)) {
                R_CATCH(ResultKvdbKeyNotFound) {
                    return ResultNcmContentMetaNotFound;
                }
            } R_END_TRY_CATCH;
            
            return ResultSuccess;
        }

        Result GetContentMetaValuePointer(const void **out_value_ptr, size_t *out_size, const ContentMetaKey &key, const kvdb::MemoryKeyValueStore<ContentMetaKey> *kvs) {
            R_TRY(GetContentMetaSize(out_size, key, kvs));
            R_TRY(kvs->GetValuePointer(out_value_ptr, key));
            return ResultSuccess;
        }

    }

    Result ContentMetaDatabaseInterface::GetContentIdByTypeImpl(ContentId* out, const ContentMetaKey& key, ContentType type, std::optional<u8> id_offset) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        const auto it = this->store->lower_bound(key);
        if (it == this->store->end() || it->GetKey().id != key.id) {
            return ResultNcmContentMetaNotFound;
        }

        const auto stored_key = it->GetKey();
        const u8* value = nullptr;
        size_t value_size;

        R_TRY(GetContentMetaValuePointer(reinterpret_cast<const void**>(&value), &value_size, stored_key, this->store));
        const auto header = reinterpret_cast<const InstallContentMetaHeader*>(value);

        if (header->content_count == 0) {
            return ResultNcmContentNotFound;
        }

        const ContentInfo* content_infos = reinterpret_cast<const ContentInfo*>(value + sizeof(InstallContentMetaHeader) + header->extended_header_size);
        const ContentInfo* found_content_info = nullptr;

        if (id_offset) {
            for (size_t i = 0; i < header->content_count; i++) {
                const ContentInfo* content_info = &content_infos[i];

                if (content_info->content_type == type && content_info->id_offset == *id_offset) {
                    found_content_info = content_info;
                    break;
                }
            }
        } else {
            const ContentInfo* lowest_id_offset_info = nullptr;

            for (size_t i = 0; i < header->content_count; i++) {
                const ContentInfo* content_info = &content_infos[i];

                if (content_info->content_type == type && (!lowest_id_offset_info || lowest_id_offset_info->id_offset > content_info->id_offset)) {
                    lowest_id_offset_info = content_info;
                }
            }

            found_content_info = lowest_id_offset_info;
        }

        if (!found_content_info) {
            return ResultNcmContentNotFound;
        }

        *out = found_content_info->content_id;
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Set(ContentMetaKey key, InBuffer<u8> value) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        R_TRY(this->store->Set(key, value.buffer, value.num_elements));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Get(Out<u64> out_size, ContentMetaKey key, OutBuffer<u8> out_value) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        R_TRY(this->store->Get(out_size.GetPointer(), out_value.buffer, out_value.num_elements, key));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Remove(ContentMetaKey key) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        R_TRY(this->store->Remove(key));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::GetContentIdByType(Out<ContentId> out_content_id, ContentMetaKey key, ContentType type) {
        ContentId content_id;
        R_TRY(this->GetContentIdByTypeImpl(&content_id, key, type, std::nullopt));
        out_content_id.SetValue(content_id);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::ListContentInfo(Out<u32> out_entries_read, OutBuffer<ContentInfo> out_info, ContentMetaKey key, u32 start_index) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::List(Out<u32> out_entries_total, Out<u32> out_entries_written, OutBuffer<ContentInfo> out_info, ContentMetaType meta_type, TitleId application_title_id, TitleId title_id_min, TitleId title_id_max, ContentMetaAttribute attributes) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetLatestContentMetaKey(Out<ContentMetaKey> out_key, TitleId title_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::ListApplication(Out<u32> out_entries_total, Out<u32> out_entries_written, OutBuffer<ApplicationContentMetaKey> out_keys, ContentMetaType meta_type) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::Has(Out<bool> out, ContentMetaKey key) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::HasAll(Out<bool> out, InBuffer<ContentMetaKey> keys) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetSize(Out<u64> out_size, ContentMetaKey key) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetRequiredSystemVersion(Out<u32> out_version, ContentMetaKey key) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetPatchId(Out<TitleId> out_patch_id, ContentMetaKey key) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::DisableForcibly() {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::LookupOrphanContent(OutBuffer<bool> out_orphaned, InBuffer<ContentId> content_ids) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::Commit() {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::HasContent(Out<bool> out, ContentMetaKey key, ContentId content_id) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::ListContentMetaInfo(Out<u32> out_entries_written, OutBuffer<ContentMetaInfo> out_info, ContentMetaKey key, u32 start_index) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetAttributes(Out<ContentMetaAttribute> out_attributes, ContentMetaKey key) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetRequiredApplicationVersion(Out<u32> out_version, ContentMetaKey key) {
        return ResultKernelConnectionClosed;
    }

    Result ContentMetaDatabaseInterface::GetContentIdByTypeAndIdOffset(Out<ContentId> out_content_id, ContentMetaKey key, ContentType type, u8 id_offset) {
        ContentId content_id;
        R_TRY(this->GetContentIdByTypeImpl(&content_id, key, type, std::optional(id_offset)));
        out_content_id.SetValue(content_id);
        return ResultSuccess;
    }

}