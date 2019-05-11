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
    u8 uuid[0x10];
};

static_assert(sizeof(Uuid) == 0x10, "Uuid definition!");

static constexpr Uuid InvalidUuid = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

typedef Uuid ContentId;
typedef Uuid PlaceHolderId;
typedef Uuid RightsId;



class PlaceHolderAccessor {
    public:
        class CacheEntry {
            public:
                PlaceHolderId id;
                FILE* handle;
                u64 counter;
        };

    public:
        static constexpr size_t MaxCaches = 0x2;

        CacheEntry caches[MaxCaches];
        char* root_path;
        u64 cur_counter;
        HosMutex cache_mutex;
        std::function<void (char* out, PlaceHolderId placeholder_id, const char* root)> make_placeholder_path_func;
        bool delay_flush;

        PlaceHolderAccessor() : cur_counter(0), delay_flush(false) {
            for (size_t i = 0; i < MaxCaches; i++) {
                caches[i].id = InvalidUuid;
            }
        }

        inline void GetPlaceHolderRootPath(char* placeholder_root_out) {
            /* TODO: Replace with BoundedString? */
            snprintf(placeholder_root_out, FS_MAX_PATH, "%.48s%s", this->root_path, "/placehld");
        }

        inline void GetPlaceHolderPath(char* placeholder_path_out, PlaceHolderId placeholder_id) {
            char placeholder_root_path[FS_MAX_PATH] = {0};
            this->GetPlaceHolderRootPath(placeholder_root_path);
            this->make_placeholder_path_func(placeholder_path_out, placeholder_id, placeholder_root_path);
        }

        void GetPlaceHolderPathUncached(char* placeholder_path_out, PlaceHolderId placeholder_id);
        Result Create(PlaceHolderId placeholder_id, size_t size);
        Result Delete(PlaceHolderId placeholder_id);
        Result Open(FILE** out_handle, PlaceHolderId id);

        CacheEntry *FindInCache(PlaceHolderId placeholder_id);
        bool LoadFromCache(FILE** out_handle, PlaceHolderId placeholder_id);
        void StoreToCache(FILE* handle, PlaceHolderId placeholder_id);
        void ClearAllCaches();
};

class ContentUtils {
};