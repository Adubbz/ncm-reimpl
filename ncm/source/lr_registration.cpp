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

char* Registration::RedirectPath(std::list<std::shared_ptr<LocationListEntry>>* list, u64 tid, const char* path, bool is_application)
{
    char* out = NULL;

    // Remove any existing entries with the current tid
    for (auto it = list->begin(); it != list->end();) 
    {
        std::shared_ptr<LocationListEntry> entry = *it;

        if (entry->tid == tid)
            it = list->erase(it);
        else 
            it++;
    }

    auto new_entry = std::make_shared<LocationListEntry>();
    new_entry->tid = tid;
    out = (char*)memcpy(new_entry->content_path, path, FS_MAX_PATH);
    list->push_back(new_entry);
    return out;
}