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

class PathUtils {
    public:
        static void GetContentFileName(char* out, ContentId content_id);
        static void GetPlaceHolderFileName(char* out, PlaceHolderId placeholder_id);
        static bool IsHexChar(char c);
        static bool IsNcaPath(const char* path);
};

class PathView {
    private:
        std::string_view path; /* Nintendo uses nn::util::string_view here. */    
    public:
        PathView(std::string_view p) : path(p) { /* ...*/ }
        bool HasPrefix(std::string_view prefix) const;
        bool HasSuffix(std::string_view suffix) const;
        std::string_view GetFileName() const;
};