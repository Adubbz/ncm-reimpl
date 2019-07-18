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

namespace sts::lr {

    class LocationResolverBase : public IServiceObject {
        protected:
            enum class CommandId {
                ResolveProgramPath = 0,
                RedirectProgramPath = 1,
                ResolveApplicationControlPath = 2,
                ResolveApplicationHtmlDocumentPath = 3,
                ResolveDataPath = 4,
                RedirectApplicationControlPath = 5,
                RedirectApplicationHtmlDocumentPath = 6,
                ResolveApplicationLegalInformationPath = 7,
                RedirectApplicationLegalInformationPath = 8,
                Refresh = 9,
                RedirectApplicationProgramPath = 10,
                ClearApplicationRedirection = 11,
                EraseProgramRedirection = 12,
                EraseApplicationControlRedirection = 13,
                EraseApplicationHtmlDocumentRedirection = 14,
                EraseApplicationLegalInformationRedirection = 15,
                ResolveProgramPathForDebug = 16,
                RedirectProgramPathForDebug = 17,
                RedirectApplicationProgramPathForDebug = 18,
                EraseProgramRedirectionForDebug = 19,
            };
        private:
            std::list<std::shared_ptr<reg::LocationListEntry>> program_location_list;
            std::list<std::shared_ptr<reg::LocationListEntry>> app_control_location_list;
            std::list<std::shared_ptr<reg::LocationListEntry>> html_docs_location_list;
            std::list<std::shared_ptr<reg::LocationListEntry>> legal_info_location_list;
            ncm::StorageId storage_id;
            u64* content_meta_database;
            u64* content_storage;
        protected:
            LocationResolverBase(ncm::StorageId storage_id);
        public:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid);
            virtual Result RedirectProgramPath(ncm::TitleId tid, InPointer<const char> path);
            virtual Result ResolveApplicationControlPath(OutPointerWithClientSize<char> out, ncm::TitleId tid);
            virtual Result ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, ncm::TitleId tid);
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid);
            virtual Result RedirectApplicationControlPath(ncm::TitleId tid, InPointer<const char> path);
            virtual Result RedirectApplicationHtmlDocumentPath(ncm::TitleId tid, InPointer<const char> path);
            virtual Result ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, ncm::TitleId tid);
            virtual Result RedirectApplicationLegalInformationPath(ncm::TitleId tid, InPointer<const char> path);
            virtual Result Refresh();
            virtual Result RedirectApplicationProgramPath(ncm::TitleId tid, InPointer<const char> path);
            virtual Result ClearApplicationRedirection();
            virtual Result EraseProgramRedirection(ncm::TitleId tid);
            virtual Result EraseApplicationControlRedirection(ncm::TitleId tid);
            virtual Result EraseApplicationHtmlDocumentRedirection(ncm::TitleId tid);
            virtual Result EraseApplicationLegalInformationRedirection(ncm::TitleId tid);
        public:
            // Compile error without this
            DEFINE_SERVICE_DISPATCH_TABLE {
                /* No entries, because LocationResolverBase is abstract. */
            };
    };

    class LocationResolverService : public LocationResolverBase {
        public:
            u32 unk_0x60 = 1;

            LocationResolverService(ncm::StorageId storage_id);

            Result RefreshImpl();
        protected:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result Refresh() override;
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(LocationResolverService, ResolveProgramPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, RedirectProgramPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, ResolveApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, ResolveApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, ResolveDataPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, RedirectApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, RedirectApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, ResolveApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, RedirectApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, Refresh),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, RedirectApplicationProgramPath,              FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, ClearApplicationRedirection,                 FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, EraseProgramRedirection,                     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, EraseApplicationControlRedirection,          FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, EraseApplicationHtmlDocumentRedirection,     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverService, EraseApplicationLegalInformationRedirection, FirmwareVersion_500),
            };
    };

    class HostLocationResolverService : public LocationResolverBase {
        public:
            HostLocationResolverService(ncm::StorageId storage_id);
        protected:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result Refresh() override;
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, ResolveProgramPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, RedirectProgramPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, ResolveApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, ResolveApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, ResolveDataPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, RedirectApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, RedirectApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, ResolveApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, RedirectApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, Refresh),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, RedirectApplicationProgramPath,              FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, ClearApplicationRedirection,                 FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, EraseProgramRedirection,                     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, EraseApplicationControlRedirection,          FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, EraseApplicationHtmlDocumentRedirection,     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverService, EraseApplicationLegalInformationRedirection, FirmwareVersion_500),
            };
    };

}