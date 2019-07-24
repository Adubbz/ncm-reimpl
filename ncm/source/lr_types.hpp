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

namespace sts::lr {

    struct Path {
        char path[FS_MAX_PATH-1];

        Path(const char* path) {
            std::memcpy(this->path, path, FS_MAX_PATH-1);
        }

        Path& operator=(const Path& other) {
            std::memcpy(this->path, other.path, FS_MAX_PATH-1);
            return *this;
        }
    };

}