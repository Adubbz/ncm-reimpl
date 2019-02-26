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

enum LrLocationResolverCmd : u32
{
    LrLocationResolver_Cmd_ResolveProgramPath = 0,
    LrLocationResolver_Cmd_RedirectProgramPath = 1,
    LrLocationResolver_Cmd_ResolveApplicationControlPath = 2,
    LrLocationResolver_Cmd_ResolveApplicationHtmlDocumentPath = 3,
    LrLocationResolver_Cmd_ResolveDataPath = 4,
    LrLocationResolver_Cmd_RedirectApplicationControlPath = 5,
    LrLocationResolver_Cmd_RedirectApplicationHtmlDocumentPath = 6,
    LrLocationResolver_Cmd_ResolveApplicationLegalInformationPath = 7,
    LrLocationResolver_Cmd_RedirectApplicationLegalInformationPath = 8,
    LrLocationResolver_Cmd_Refresh = 9,
    LrLocationResolver_Cmd_RedirectProgramPath2 = 10,
    LrLocationResolver_Cmd_Refresh2 = 11,
    LrLocationResolver_Cmd_DeleteProgramPath = 12,
    LrLocationResolver_Cmd_DeleteApplicationControlPath = 13,
    LrLocationResolver_Cmd_DeleteApplicationHtmlDocumentPath = 14,
    LrLocationResolver_Cmd_DeleteApplicationLegalInformationPath = 15,
};

class ILocationResolverInterface : public IServiceObject
{
    private:
        virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result RedirectProgramPath(u64 tid, InPointer<const char> path) = 0;
        virtual Result ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result RedirectApplicationControlPath(u64 tid, InPointer<const char> path) = 0;
        virtual Result RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path) = 0;
        virtual Result ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path) = 0;
        virtual Result Refresh() = 0;
        virtual Result RedirectProgramPath2(u64 tid, InPointer<const char> path) = 0;
        virtual Result Refresh2() = 0;
        virtual Result DeleteProgramPath(u64 tid) = 0;
        virtual Result DeleteApplicationControlPath(u64 tid) = 0;
        virtual Result DeleteApplicationHtmlDocumentPath(u64 tid) = 0;
        virtual Result DeleteApplicationLegalInformationPath(u64 tid) = 0;

    public:
        DEFINE_SERVICE_DISPATCH_TABLE 
        {
            /* 1.0.0- */
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveProgramPath, &ILocationResolverInterface::ResolveProgramPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectProgramPath, &ILocationResolverInterface::RedirectProgramPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveApplicationControlPath, &ILocationResolverInterface::ResolveApplicationControlPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveApplicationHtmlDocumentPath, &ILocationResolverInterface::ResolveApplicationHtmlDocumentPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveDataPath, &ILocationResolverInterface::ResolveDataPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationControlPath, &ILocationResolverInterface::RedirectApplicationControlPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationHtmlDocumentPath, &ILocationResolverInterface::RedirectApplicationHtmlDocumentPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveApplicationLegalInformationPath, &ILocationResolverInterface::ResolveApplicationLegalInformationPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationLegalInformationPath, &ILocationResolverInterface::RedirectApplicationLegalInformationPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_Refresh, &ILocationResolverInterface::Refresh>(),

            /* 5.0.0- */
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectProgramPath2, &ILocationResolverInterface::RedirectProgramPath2, FirmwareVersion_500>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_Refresh2, &ILocationResolverInterface::Refresh2>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_DeleteProgramPath, &ILocationResolverInterface::DeleteProgramPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_DeleteApplicationControlPath, &ILocationResolverInterface::DeleteApplicationControlPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_DeleteApplicationHtmlDocumentPath, &ILocationResolverInterface::DeleteApplicationHtmlDocumentPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_DeleteApplicationLegalInformationPath, &ILocationResolverInterface::DeleteApplicationLegalInformationPath>(),
        };
};

class LocationResolverInterface : public ILocationResolverInterface
{

};

class HostLocationResolverInterface : public ILocationResolverInterface
{

};