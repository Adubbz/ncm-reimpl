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
        struct LocationListEntry
        {
            LocationListEntry* prev_entry;
            LocationListEntry* next_entry;
            u64 tid;
            char content_path[0x300];
            u8 is_application;
        };

        struct LocationList
        {
            LocationListEntry* last_entry;
            LocationListEntry* first_entry;
        };

        LocationList program_location_list;
        LocationList app_control_location_list;
        LocationList html_docs_location_list;
        LocationList legal_info_location_lost;
        FsStorageId storage_id;
        u64* content_meta_database;
        u64* content_storage;

        LocationResolverBase(FsStorageId storage_id);

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
        virtual Result RedirectApplicationProgramPath(u64 tid, InPointer<const char> path) = 0;
        virtual Result ClearApplicationRedirection() = 0;
        virtual Result EraseProgramRedirection(u64 tid) = 0;
        virtual Result EraseApplicationControlRedirection(u64 tid) = 0;
        virtual Result EraseApplicationHtmlDocumentRedirection(u64 tid) = 0;
        virtual Result EraseApplicationLegalInformationRedirection(u64 tid) = 0;

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
        
    private:
        virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result RedirectProgramPath(u64 tid, InPointer<const char> path) override;
        virtual Result ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result RedirectApplicationControlPath(u64 tid, InPointer<const char> path) override;
        virtual Result RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path) override;
        virtual Result ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path) override;
        virtual Result Refresh() override;
        virtual Result RedirectApplicationProgramPath(u64 tid, InPointer<const char> path) override;
        virtual Result ClearApplicationRedirection() override;
        virtual Result EraseProgramRedirection(u64 tid) override;
        virtual Result EraseApplicationControlRedirection(u64 tid) override;
        virtual Result EraseApplicationHtmlDocumentRedirection(u64 tid) override;
        virtual Result EraseApplicationLegalInformationRedirection(u64 tid) override;
};

class HostLocationResolver : public LocationResolverBase
{
    public:
        HostLocationResolver(FsStorageId storage_id);

    private:
        virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result RedirectProgramPath(u64 tid, InPointer<const char> path) override;
        virtual Result ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result RedirectApplicationControlPath(u64 tid, InPointer<const char> path) override;
        virtual Result RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path) override;
        virtual Result ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid) override;
        virtual Result RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path) override;
        virtual Result Refresh() override;
        virtual Result RedirectApplicationProgramPath(u64 tid, InPointer<const char> path) override;
        virtual Result ClearApplicationRedirection() override;
        virtual Result EraseProgramRedirection(u64 tid) override;
        virtual Result EraseApplicationControlRedirection(u64 tid) override;
        virtual Result EraseApplicationHtmlDocumentRedirection(u64 tid) override;
        virtual Result EraseApplicationLegalInformationRedirection(u64 tid) override;
};