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

enum LrRegisteredLrCmd : u32
{
    LrRegisteredLr_Cmd_ResolveProgramPath = 0,
    LrRegisteredLr_Cmd_RegisterProgramPath = 1,
    LrRegisteredLr_Cmd_UnregisterProgramPath = 2,
    LrRegisteredLr_Cmd_RedirectProgramPath = 3,
    LrRegisteredLr_Cmd_ResolveHtmlDocumentPath = 4,
    LrRegisteredLr_Cmd_RegisterHtmlDocumentPath = 5,
    LrRegisteredLr_Cmd_UnregisterHtmlDocumentPath = 6,
    LrRegisteredLr_Cmd_RedirectHtmlDocumentPath = 7,
};

class RegisteredLocationResolverInterface : public IServiceObject
{
    private:
        Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid);
        Result RegisterProgramPath(u64 tid, InPointer<const char> path);
        Result UnregisterProgramPath(u64 tid);
        Result RedirectProgramPath(u64 tid, InPointer<const char> path);
        Result ResolveHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid);
        Result RegisterHtmlDocumentPath(u64 tid, InPointer<const char> path);
        Result UnregisterHtmlDocumentPath(u64 tid);
        Result RedirectHtmlDocumentPath(u64 tid, InPointer<const char> path);
    
    public:
        DEFINE_SERVICE_DISPATCH_TABLE 
        {
            /* 1.0.0- */
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_ResolveProgramPath, &RegisteredLocationResolverInterface::ResolveProgramPath>(),
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_RegisterProgramPath, &RegisteredLocationResolverInterface::RegisterProgramPath>(),
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_UnregisterProgramPath, &RegisteredLocationResolverInterface::UnregisterProgramPath>(),
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_RedirectProgramPath, &RegisteredLocationResolverInterface::RedirectProgramPath>(),
                
            /* 2.0.0- */
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_ResolveHtmlDocumentPath, &RegisteredLocationResolverInterface::ResolveHtmlDocumentPath, FirmwareVersion_200>(),
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_RegisterHtmlDocumentPath, &RegisteredLocationResolverInterface::RegisterHtmlDocumentPath, FirmwareVersion_200>(),
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_UnregisterHtmlDocumentPath, &RegisteredLocationResolverInterface::UnregisterHtmlDocumentPath, FirmwareVersion_200>(),
            MakeServiceCommandMeta<LrRegisteredLr_Cmd_RedirectHtmlDocumentPath, &RegisteredLocationResolverInterface::RedirectHtmlDocumentPath, FirmwareVersion_200>(),
        };
};