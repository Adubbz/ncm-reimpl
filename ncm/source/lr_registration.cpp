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

#include "lr_registration.hpp"

namespace sts::lr::reg {

    char* RedirectPath(std::list<std::shared_ptr<LocationListEntry>>* list, u64 tid, const char* path, bool is_application) {
        char* out = NULL;

        // Remove any existing entries with the current tid
        for (auto it = list->begin(); it != list->end();)  {
            std::shared_ptr<LocationListEntry> entry = *it;

            if (entry->tid == tid) {
                it = list->erase(it);
            } else {
                it++;
            }
        }

        auto new_entry = std::make_shared<LocationListEntry>();
        new_entry->tid = tid;
        out = (char*)memcpy(new_entry->content_path, path, FS_MAX_PATH-1);
        list->push_back(new_entry);
        return out;
    }

    bool ResolvePath(std::list<std::shared_ptr<LocationListEntry>>* list, char* path_out, u64 tid) {
        if (list->empty()) {
            return false;
        }

        std::shared_ptr<LocationListEntry> out_entry = nullptr;

        for (auto& entry : *list) {
            if (entry->tid == tid) {
                out_entry = entry;
            }
        }

        if (!out_entry)
            return false;

        memcpy(path_out, out_entry->content_path, FS_MAX_PATH-1);
        return true;
    }

    void EraseRedirectionWithTid(std::list<std::shared_ptr<LocationListEntry>>* list, u64 tid) {
        if (list->empty()) {
            return;
        }

        for (auto it = list->begin(); it != list->end();) {
            std::shared_ptr<LocationListEntry> entry = *it;

            if (entry->tid == tid) {
                it = list->erase(it);
            } else {
                it++;
            }
        }
    }

    void EraseRedirectionWithMask(std::list<std::shared_ptr<LocationListEntry>>* list, int mask) {
        if (list->empty()) {
            return;
        }

        for (auto it = list->begin(); it != list->end();) {
            std::shared_ptr<LocationListEntry> entry = *it;

            // Remove the entry if there are no bits set that aren't in the mask
            if (!(entry->is_application & ~mask)) {
                it = list->erase(it);
            }
            else {
                it++;
            }
        }
    }

}