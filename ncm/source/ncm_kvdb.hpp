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

    template <typename Key, typename Value>
    class FlatMapKeyValueStore {
        public:
            class KeyValue {
                public:
                    Key key;
                    Value* value_buf;
                    u64 value_size;
            };

            class Index {
                public:
                    u32 count;
                    u32 max;
                    KeyValue* entries;
            };

            Index index;
            char archive_path[FS_MAX_PATH-1];
    };

    struct KeyValueArchive {
        bool is_present;
        u64 max_working_buffer_size;
    };

    template <typename Key, typename Value>
    class KvdbStore {
        public:
            FlatMapKeyValueStore<Key, Value> flat_map_store;
            KeyValueArchive archive;
    };

    typedef FlatMapKeyValueStore<ContentMetaKey, void>::KeyValue ContentMetaKeyValue;

}