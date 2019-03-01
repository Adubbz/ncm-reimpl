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
#include "lr_registration.hpp"

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
    LrLocationResolver_Cmd_RedirectApplicationProgramPath = 10,
    LrLocationResolver_Cmd_ClearApplicationRedirection = 11,
    LrLocationResolver_Cmd_EraseProgramRedirection = 12,
    LrLocationResolver_Cmd_EraseApplicationControlRedirection = 13,
    LrLocationResolver_Cmd_EraseApplicationHtmlDocumentRedirection = 14,
    LrLocationResolver_Cmd_EraseApplicationLegalInformationRedirection = 15,
};

class LocationResolverBase : public IServiceObject
{
    public:
        std::list<std::shared_ptr<Registration::LocationListEntry>> program_location_list;
        std::list<std::shared_ptr<Registration::LocationListEntry>> app_control_location_list;
        std::list<std::shared_ptr<Registration::LocationListEntry>> html_docs_location_list;
        std::list<std::shared_ptr<Registration::LocationListEntry>> legal_info_location_list;
        FsStorageId storage_id;
        u64* content_meta_database;
        u64* content_storage;

        LocationResolverBase(FsStorageId storage_id);

    public:
        virtual Result RedirectProgramPath(u64 tid, InPointer<const char> path);
        virtual Result ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid);
        virtual Result ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid);
        virtual Result RedirectApplicationControlPath(u64 tid, InPointer<const char> path);
        virtual Result RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path);
        virtual Result ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid);
        virtual Result RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path);
        virtual Result RedirectApplicationProgramPath(u64 tid, InPointer<const char> path);
        virtual Result ClearApplicationRedirection();
        virtual Result EraseProgramRedirection(u64 tid);
        virtual Result EraseApplicationControlRedirection(u64 tid);
        virtual Result EraseApplicationHtmlDocumentRedirection(u64 tid);
        virtual Result EraseApplicationLegalInformationRedirection(u64 tid);

    protected:
        virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
        virtual Result Refresh() = 0;

    public:
        DEFINE_SERVICE_DISPATCH_TABLE 
        {
            /* 1.0.0- */
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveProgramPath, &LocationResolverBase::ResolveProgramPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectProgramPath, &LocationResolverBase::RedirectProgramPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveApplicationControlPath, &LocationResolverBase::ResolveApplicationControlPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveApplicationHtmlDocumentPath, &LocationResolverBase::ResolveApplicationHtmlDocumentPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveDataPath, &LocationResolverBase::ResolveDataPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationControlPath, &LocationResolverBase::RedirectApplicationControlPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationHtmlDocumentPath, &LocationResolverBase::RedirectApplicationHtmlDocumentPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ResolveApplicationLegalInformationPath, &LocationResolverBase::ResolveApplicationLegalInformationPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationLegalInformationPath, &LocationResolverBase::RedirectApplicationLegalInformationPath>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_Refresh, &LocationResolverBase::Refresh>(),

            /* 5.0.0- */
            MakeServiceCommandMeta<LrLocationResolver_Cmd_RedirectApplicationProgramPath, &LocationResolverBase::RedirectApplicationProgramPath, FirmwareVersion_500>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_ClearApplicationRedirection, &LocationResolverBase::ClearApplicationRedirection>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_EraseProgramRedirection, &LocationResolverBase::EraseProgramRedirection>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_EraseApplicationControlRedirection, &LocationResolverBase::EraseApplicationControlRedirection>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_EraseApplicationHtmlDocumentRedirection, &LocationResolverBase::EraseApplicationHtmlDocumentRedirection>(),
            MakeServiceCommandMeta<LrLocationResolver_Cmd_EraseApplicationLegalInformationRedirection, &LocationResolverBase::EraseApplicationLegalInformationRedirection>(),
        };
};

class LocationResolver : public LocationResolverBase
{
    public:
        u32 unk_0x60 = 1;

        LocationResolver(FsStorageId storage_id);

        Result RefreshImpl();
        
    protected:
        virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result Refresh() override;
};

class HostLocationResolver : public LocationResolverBase
{
    public:
        HostLocationResolver(FsStorageId storage_id);

    protected:
        virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result Refresh() override;
};