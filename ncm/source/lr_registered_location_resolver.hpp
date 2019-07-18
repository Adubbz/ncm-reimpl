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

    class RegisteredLocationResolverService final : public IServiceObject {
        protected:
            enum class CommandId {
                ResolveProgramPath = 0,
                RegisterProgramPath = 1,
                UnregisterProgramPath = 2,
                RedirectProgramPath = 3,
                ResolveHtmlDocumentPath = 4,
                RegisterHtmlDocumentPath = 5,
                UnregisterHtmlDocumentPath = 6,
                RedirectHtmlDocumentPath = 7,
                Refresh = 8,
            };
        public:
            Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid);
            Result RegisterProgramPath(u64 tid, InPointer<const char> path);
            Result UnregisterProgramPath(u64 tid);
            Result RedirectProgramPath(u64 tid, InPointer<const char> path);
            Result ResolveHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid);
            Result RegisterHtmlDocumentPath(u64 tid, InPointer<const char> path);
            Result UnregisterHtmlDocumentPath(u64 tid);
            Result RedirectHtmlDocumentPath(u64 tid, InPointer<const char> path);
            Result Refresh();
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, ResolveProgramPath),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, RegisterProgramPath),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, UnregisterProgramPath),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, RedirectProgramPath),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, ResolveHtmlDocumentPath,    FirmwareVersion_200),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, RegisterHtmlDocumentPath,   FirmwareVersion_200),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, UnregisterHtmlDocumentPath, FirmwareVersion_200),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, RedirectHtmlDocumentPath,   FirmwareVersion_200),
                MAKE_SERVICE_COMMAND_META(RegisteredLocationResolverService, Refresh,                    FirmwareVersion_700),
            };
    };

}