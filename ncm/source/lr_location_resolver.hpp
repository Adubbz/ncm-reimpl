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

    class LocationResolverInterface : public LocationResolverBase {
        public:
            u32 unk_0x60 = 1;

            LocationResolverInterface(ncm::StorageId storage_id);

            Result RefreshImpl();
        protected:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result Refresh() override;
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, ResolveProgramPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, RedirectProgramPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, ResolveApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, ResolveApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, ResolveDataPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, RedirectApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, RedirectApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, ResolveApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, RedirectApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, Refresh),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, RedirectApplicationProgramPath,              FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, ClearApplicationRedirection,                 FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, EraseProgramRedirection,                     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, EraseApplicationControlRedirection,          FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, EraseApplicationHtmlDocumentRedirection,     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(LocationResolverInterface, EraseApplicationLegalInformationRedirection, FirmwareVersion_500),
            };
    };

    class HostLocationResolverInterface : public LocationResolverBase {
        public:
            HostLocationResolverInterface(ncm::StorageId storage_id);
        protected:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) override;
            virtual Result Refresh() override;
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, ResolveProgramPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, RedirectProgramPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, ResolveApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, ResolveApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, ResolveDataPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, RedirectApplicationControlPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, RedirectApplicationHtmlDocumentPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, ResolveApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, RedirectApplicationLegalInformationPath),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, Refresh),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, RedirectApplicationProgramPath,              FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, ClearApplicationRedirection,                 FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, EraseProgramRedirection,                     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, EraseApplicationControlRedirection,          FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, EraseApplicationHtmlDocumentRedirection,     FirmwareVersion_500),
                MAKE_SERVICE_COMMAND_META(HostLocationResolverInterface, EraseApplicationLegalInformationRedirection, FirmwareVersion_500),
            };
    };

}