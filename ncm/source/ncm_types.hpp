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

namespace sts::ncm {

    struct Uuid {
        u8 uuid[0x10];
    };

    static_assert(sizeof(Uuid) == 0x10, "Uuid definition!");

    static constexpr Uuid InvalidUuid = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };

    typedef Uuid ContentId;
    typedef Uuid PlaceHolderId;

    typedef void (*MakeContentPathFunc)(char* out, ContentId content_id, const char* root);
    typedef void (*MakePlaceHolderPathFunc)(char* out, PlaceHolderId placeholder_id, const char* root);

    // TODO: Remove this
    static constexpr Result ResultNcmInvalidPlaceHolderDirectoryEntry = MAKERESULT(Module_Ncm, 170);

}