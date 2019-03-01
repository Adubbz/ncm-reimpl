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
#include <list>

#include <switch.h>
#include <stratosphere.hpp>

class Registration
{
    public:
        struct LocationListEntry
        {
            u64 tid;
            char content_path[FS_MAX_PATH];
            u8 is_application;
        };

        static char* RedirectPath(std::list<std::shared_ptr<LocationListEntry>>* list, u64 tid, const char* path, bool is_application);
        static bool ResolvePath(std::list<std::shared_ptr<LocationListEntry>>* list, char* path_out, u64 tid);
        static void EraseRedirection(std::list<std::shared_ptr<LocationListEntry>>* list, u64 tid);
};