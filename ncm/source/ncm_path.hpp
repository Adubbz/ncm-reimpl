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
#include <sys/dirent.h>
#include "ncm_types.hpp"

class PathUtils {
    public:
        static void MakeContentPathUnlayered(char* out_path, ContentId content_id, const char* root);
        static void MakeContentPathHashByteLayered(char* out_path, ContentId content_id, const char* root);
        static void MakeContentPath10BitLayered(char* out_path, ContentId content_id, const char* root);
        static void MakeContentPathDualLayered(char* out_path, ContentId content_id, const char* root);

        static void MakePlaceHolderPathUnlayered(char* out_path, PlaceHolderId placeholder_id, const char* root);
        static void MakePlaceHolderPathHashByteLayered(char* out_path, PlaceHolderId placeholder_id, const char* root);

        static void GetStringFromContentId(char* out, ContentId content_id);
        static void GetContentFileName(char* out, ContentId content_id);
        static void GetStringFromPlaceHolderId(char* out, PlaceHolderId placeholder_id);
        static void GetPlaceHolderFileName(char* out, PlaceHolderId placeholder_id);

        static unsigned int GetDirLevelForContentPathFunc(MakeContentPathFunc content_path_func);
        static unsigned int GetDirLevelForPlaceHolderPathFunc(MakePlaceHolderPathFunc placeholder_path_func);

        static Result GetPlaceHolderIdFromDirEntry(PlaceHolderId* out, struct dirent* dir_entry);
};