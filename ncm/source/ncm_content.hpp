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

struct Uuid {
    u8 c[0x10];
};

static constexpr Uuid InvalidUuid = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

struct ContentId {
    Uuid uuid;
};

struct PlaceHolderId {
    Uuid uuid;
};

class PlaceHolderAccessorCache {
    public:
        PlaceHolderId id;
        FILE* handle;
        u64 counter;
};

class PlaceHolderAccessor {
    public:
        static constexpr size_t MaxCaches = 0x2;

        PlaceHolderAccessorCache caches[MaxCaches];
        char* root_path;
        u64 cur_counter;
        HosMutex cache_mutex;
        std::function<void (char* out, PlaceHolderId placeholder_id, const char* root)> make_placeholder_path_func;
        bool delay_flush;

        PlaceHolderAccessor() : cur_counter(0), delay_flush(false) {
            for (size_t i = 0; i < MaxCaches; i++) {
                caches[i].id.uuid = InvalidUuid;
            }
        }
};