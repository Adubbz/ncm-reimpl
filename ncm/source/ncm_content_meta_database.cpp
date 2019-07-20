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
            ContentMetaAttribute attributes;
            u8 padding;
        };

        static_assert(sizeof(InstallContentMetaHeader) == 0x8, "InstallContentMetaHeader definition!");

        struct ApplicationMetaExtendedHeader {
            TitleId patch_id;
            u32 required_system_version;
            u32 padding;
        };

        struct PatchMetaExtendedHeader {
            TitleId application_id;
            u32 required_system_version;
            u32 extended_data_size;
            u8 reserved[0x8];
        };

        struct AddOnContentMetaExtendedHeader {
            TitleId application_id;
            u32 required_application_version;
            u32 padding;
        };

        struct SystemUpdateMetaExtendedHeader {
            u32 extended_data_size;
        };

        static_assert(sizeof(InstallContentMetaHeader) == 0x8, "InstallContentMetaHeader definition!");

        inline const InstallContentMetaHeader* GetValueHeader(const void* value) {
            return reinterpret_cast<const InstallContentMetaHeader*>(value);
        }

        template<typename ExtendedHeaderType>
        inline const ExtendedHeaderType* GetValueExtendedHeader(const void* value) {
            return reinterpret_cast<const ExtendedHeaderType*>(reinterpret_cast<const u8*>(value) + sizeof(InstallContentMetaHeader));
        }

        inline const ContentInfo* GetValueContentInfos(const void* value) {
            return reinterpret_cast<const ContentInfo*>(reinterpret_cast<const u8*>(value) + sizeof(InstallContentMetaHeader) + GetValueHeader(value)->extended_header_size);
        }

        inline const ContentMetaInfo* GetValueContentMetaInfos(const void* value) {
            auto header = GetValueHeader(value);
            return reinterpret_cast<const ContentMetaInfo*>(reinterpret_cast<const u8*>(GetValueContentInfos(value)) + sizeof(ContentInfo) * header->content_count);
        }

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

        const auto it = this->kvs->lower_bound(key);
        if (it == this->kvs->end() || it->GetKey().id != key.id) {
            return ResultNcmContentMetaNotFound;
        }

        const auto stored_key = it->GetKey();
        const void* value = nullptr;
        size_t value_size = 0;

        R_TRY(GetContentMetaValuePointer(&value, &value_size, stored_key, this->kvs));
        const auto header = GetValueHeader(value);

        if (header->content_count == 0) {
            return ResultNcmContentNotFound;
        }

        const ContentInfo* content_infos = GetValueContentInfos(value);
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

    Result ContentMetaDatabaseInterface::GetLatestContentMetaKeyImpl(ContentMetaKey* out_key, TitleId tid) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        ContentMetaKey key = {0};
        key.id = tid;

        if (this->kvs->GetCount() == 0) {
            return ResultNcmContentMetaNotFound; 
        }

        auto entry = this->kvs->lower_bound(key);
        if (entry == this->kvs->end()) {
            return ResultNcmContentMetaNotFound;
        }

        bool found_key = false;

        for (; entry != this->kvs->end(); entry++) {
            if (entry->GetKey().id != key.id) {
                break;
            }

            if (entry->GetKey().attributes != ContentMetaAttribute::None) {
                key = entry->GetKey();
                found_key = true;
            }
        }

        if (!found_key) {
            return ResultNcmContentMetaNotFound;
        }

        *out_key = key;
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Set(ContentMetaKey key, InBuffer<u8> value) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        R_TRY(this->kvs->Set(key, value.buffer, value.num_elements));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Get(Out<u64> out_size, ContentMetaKey key, OutBuffer<u8> out_value) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        R_TRY(this->kvs->Get(out_size.GetPointer(), out_value.buffer, out_value.num_elements, key));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Remove(ContentMetaKey key) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        R_TRY(this->kvs->Remove(key));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::GetContentIdByType(Out<ContentId> out_content_id, ContentMetaKey key, ContentType type) {
        ContentId content_id;
        R_TRY(this->GetContentIdByTypeImpl(&content_id, key, type, std::nullopt));
        out_content_id.SetValue(content_id);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::ListContentInfo(Out<u32> out_entries_written, OutBuffer<ContentInfo> out_info, ContentMetaKey key, u32 start_index) {
        if (start_index >> 0x1f != 0) {
            return ResultNcmInvalidOffset;
        }

        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        const void* value = nullptr;
        size_t value_size = 0;
        R_TRY(GetContentMetaValuePointer(&value, &value_size, key, this->kvs));
        const auto header = GetValueHeader(value);
        const auto content_infos = GetValueContentInfos(value);
        size_t entries_written = 0;

        if (out_info.num_elements == 0) {
            out_entries_written.SetValue(0);
            return ResultSuccess;
        }

        for (size_t i = start_index; i < out_info.num_elements; i++) {
            /* We have no more entries we can read out. */
            if (header->content_count <= start_index + i) {
                break;
            }

            out_info[i] = content_infos[i];
            entries_written = i + 1;
        }

        out_entries_written.SetValue(entries_written);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::List(Out<u32> out_entries_total, Out<u32> out_entries_written, OutBuffer<ContentMetaKey> out_info, ContentMetaType meta_type, TitleId application_title_id, TitleId title_id_min, TitleId title_id_max, ContentMetaAttribute attributes) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        size_t entries_total = 0;
        size_t entries_written = 0;

        /* If there are no entries then we've already successfully listed them all. */
        if (this->kvs->GetCount() == 0) {
            out_entries_total.SetValue(entries_total);
            out_entries_written.SetValue(entries_written);
            return ResultSuccess;
        }

        for (auto entry = this->kvs->begin(); entry != this->kvs->end(); entry++) {
            ContentMetaKey key = entry->GetKey();
            entries_total++;

            /* Check if this entry is suitable for writing. */
            if (!((static_cast<u8>(meta_type) == 0 || key.meta_type == meta_type) && (title_id_min <= key.id && key.id <= title_id_max) && (static_cast<u8>(attributes) == 0x7 || key.attributes == attributes))) {
                continue;
            }

            bool write_entry = false;
            
            if (static_cast<u64>(application_title_id) != 0) {
                const void* value = nullptr;
                size_t value_size = 0;
                R_TRY(GetContentMetaValuePointer(&value, &value_size, key, this->kvs));

                if (key.meta_type != ContentMetaType::Application && key.meta_type != ContentMetaType::Patch && key.meta_type != ContentMetaType::AddOnContent && key.meta_type != ContentMetaType::Delta) {
                    continue;
                }

                TitleId tid = key.id;
                
                switch (key.meta_type) {
                    case ContentMetaType::Application:
                        break;
                    default:
                        /* The first u64 of all non-application extended headers is the application title id. */
                        tid = *GetValueExtendedHeader<TitleId>(value);
                }

                if (tid == application_title_id) {
                    write_entry = true;
                }
            } else {
                write_entry = true;
            }

            /* Write the entry to the output buffer. */
            if (write_entry && entries_written < out_info.num_elements) {
                out_info[entries_written] = key;
                entries_written++;
            }
        }

        out_entries_total.SetValue(entries_total);
        out_entries_written.SetValue(entries_written);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::GetLatestContentMetaKey(Out<ContentMetaKey> out_key, TitleId title_id) {        
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        ContentMetaKey key;        
        R_TRY(this->GetLatestContentMetaKeyImpl(&key, title_id));
        out_key.SetValue(key);
        return ResultSuccess;
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
        this->disabled = true;
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::LookupOrphanContent(OutBuffer<bool> out_orphaned, InBuffer<ContentId> content_ids) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        if (out_orphaned.num_elements < content_ids.num_elements) {
            return ResultNcmBufferInsufficient;
        }

        /* Default to orphaned for all content ids. */
        if (out_orphaned.num_elements > 0) {
            std::fill_n(out_orphaned.buffer, out_orphaned.num_elements, true);
        }
        
        if (this->kvs->GetCount() == 0) {
            return ResultSuccess;
        }
        
        for (auto entry = this->kvs->begin(); entry != this->kvs->end(); entry++) {
            const auto value = entry->GetValuePointer();
            const auto header = GetValueHeader(value);

            if (header->content_count == 0) {
                continue;
            }

            if (content_ids.num_elements == 0) {
                continue;
            }

            const ContentInfo* content_infos = GetValueContentInfos(value);
            for (size_t i = 0; i < header->content_count; i++) {
                const ContentInfo* content_info = &content_infos[i];

                /* Check if any of this entry's content infos matches one of the provided content ids.
                   If they do, then the content id isn't orphaned. */
                for (size_t j = 0; j < content_ids.num_elements; j++) {
                    const ContentId content_id = content_ids[j];

                    if (content_id == content_info->content_id) {
                        out_orphaned[j] = false;
                        break;
                    }
                }
            }
        }

        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::Commit() {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        R_TRY(this->kvs->Save());
        R_TRY(fsdevCommitDevice(this->mount_name));
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::HasContent(Out<bool> out, ContentMetaKey key, ContentId content_id) {
        const void* value = nullptr;
        size_t value_size = 0;
        R_TRY(GetContentMetaValuePointer(&value, &value_size, key, this->kvs));
        const auto header = GetValueHeader(value);
        const ContentInfo* content_infos = GetValueContentInfos(value);

        if (header->content_count > 0) {
            for (size_t i = 0; i < header->content_count; i++) {
                const ContentInfo* content_info = &content_infos[i];

                if (content_id == content_info->content_id) {
                    out.SetValue(false);
                    return ResultSuccess;
                }
            }
        }

        out.SetValue(false);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::ListContentMetaInfo(Out<u32> out_entries_written, OutBuffer<ContentMetaInfo> out_meta_info, ContentMetaKey key, u32 start_index) {
        if (start_index >> 0x1f != 0) {
            return ResultNcmInvalidOffset;
        }

        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }
        
        const void* value = nullptr;
        size_t value_size = 0;
        R_TRY(GetContentMetaValuePointer(&value, &value_size, key, this->kvs));
        const auto header = GetValueHeader(value);
        const auto content_meta_infos = GetValueContentMetaInfos(value);
        size_t entries_written = 0;

        if (out_meta_info.num_elements == 0) {
            out_entries_written.SetValue(0);
            return ResultSuccess;
        }

        for (size_t i = start_index; i < out_meta_info.num_elements; i++) {
            /* We have no more entries we can read out. */
            if (header->content_meta_count <= start_index + i) {
                break;
            }

            out_meta_info[i] = content_meta_infos[i];
            entries_written = i + 1;
        }

        out_entries_written.SetValue(entries_written);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::GetAttributes(Out<ContentMetaAttribute> out_attributes, ContentMetaKey key) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        const void* value = nullptr;
        size_t value_size = 0;
        R_TRY(GetContentMetaValuePointer(&value, &value_size, key, this->kvs));
        const auto header = GetValueHeader(value);
        out_attributes.SetValue(header->attributes);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::GetRequiredApplicationVersion(Out<u32> out_version, ContentMetaKey key) {
        if (this->disabled) {
            return ResultNcmInvalidContentMetaDatabase;
        }

        if (key.meta_type != ContentMetaType::AddOnContent) {
            return ResultNcmInvalidContentMetaKey;
        }

        const void* value = nullptr;
        size_t value_size = 0;
        R_TRY(GetContentMetaValuePointer(&value, &value_size, key, this->kvs));
        const auto ext_header = GetValueExtendedHeader<AddOnContentMetaExtendedHeader>(value);
        out_version.SetValue(ext_header->required_application_version);
        return ResultSuccess;
    }

    Result ContentMetaDatabaseInterface::GetContentIdByTypeAndIdOffset(Out<ContentId> out_content_id, ContentMetaKey key, ContentType type, u8 id_offset) {
        ContentId content_id;
        R_TRY(this->GetContentIdByTypeImpl(&content_id, key, type, std::optional(id_offset)));
        out_content_id.SetValue(content_id);
        return ResultSuccess;
    }

}