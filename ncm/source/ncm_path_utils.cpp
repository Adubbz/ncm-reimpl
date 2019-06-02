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

#include "ncm_path_utils.hpp"
#include "ncm_utils.hpp"

void PathUtils::GetContentFileName(char* out, ContentId content_id) {
    char content_name[sizeof(ContentId)*2+1] = {0};
    NcmUtils::GetStringFromContentId(content_name, content_id);
    snprintf(out, FS_MAX_PATH-1, "%s%s", content_name, ".nca");
}

void PathUtils::GetPlaceHolderFileName(char* out, PlaceHolderId placeholder_id) {
    char placeholder_name[sizeof(PlaceHolderId)*2+1] = {0};
    NcmUtils::GetStringFromPlaceHolderId(placeholder_name, placeholder_id);
    snprintf(out, FS_MAX_PATH-1, "%s%s", placeholder_name, ".nca");
}

bool PathUtils::IsHexChar(char c) {
    /* This character is numerical. */
    if ((c - '0') < 9) {
        return true;
    }

    char lowercase_char = c | 0x20;
    
    /* Only a-f are valid hex. */
    return (lowercase_char - 'a') < 6;
}

bool PathUtils::IsNcaPath(const char* path) {
    PathView path_view(path);

    if (!path_view.HasSuffix(".nca")) {
        return false;
    }

    std::string_view file_name = path_view.GetFileName();

    if (file_name.length() != 0x24) {
        return false;
    }

    for (size_t i = 0; i < sizeof(Uuid)*2; i++) {
        if (!IsHexChar(file_name.at(i))) {
            return false;
        }
    }

    return true;
}

bool PathView::HasPrefix(std::string_view prefix) const {
    return this->path.compare(0, prefix.length(), prefix) == 0;
}

bool PathView::HasSuffix(std::string_view suffix) const {
    return this->path.compare(this->path.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::string_view PathView::GetFileName() const {
    return this->path.substr(this->path.find_last_of("/") + 1);
}