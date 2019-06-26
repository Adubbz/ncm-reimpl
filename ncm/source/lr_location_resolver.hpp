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

    class LocationResolverService : public IServiceObject {
        protected:
            std::list<std::shared_ptr<reg::LocationListEntry>> program_location_list;
            std::list<std::shared_ptr<reg::LocationListEntry>> app_control_location_list;
            std::list<std::shared_ptr<reg::LocationListEntry>> html_docs_location_list;
            std::list<std::shared_ptr<reg::LocationListEntry>> legal_info_location_list;
            FsStorageId storage_id;
            u64* content_meta_database;
            u64* content_storage;

            LocationResolverService(FsStorageId storage_id);

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
            };
        
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

            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) = 0;
            virtual Result Refresh() = 0;

        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                /* 1.0.0- */
                MakeServiceCommandMeta<CommandId::ResolveProgramPath, &LocationResolverService::ResolveProgramPath>(),
                MakeServiceCommandMeta<CommandId::RedirectProgramPath, &LocationResolverService::RedirectProgramPath>(),
                MakeServiceCommandMeta<CommandId::ResolveApplicationControlPath, &LocationResolverService::ResolveApplicationControlPath>(),
                MakeServiceCommandMeta<CommandId::ResolveApplicationHtmlDocumentPath, &LocationResolverService::ResolveApplicationHtmlDocumentPath>(),
                MakeServiceCommandMeta<CommandId::ResolveDataPath, &LocationResolverService::ResolveDataPath>(),
                MakeServiceCommandMeta<CommandId::RedirectApplicationControlPath, &LocationResolverService::RedirectApplicationControlPath>(),
                MakeServiceCommandMeta<CommandId::RedirectApplicationHtmlDocumentPath, &LocationResolverService::RedirectApplicationHtmlDocumentPath>(),
                MakeServiceCommandMeta<CommandId::ResolveApplicationLegalInformationPath, &LocationResolverService::ResolveApplicationLegalInformationPath>(),
                MakeServiceCommandMeta<CommandId::RedirectApplicationLegalInformationPath, &LocationResolverService::RedirectApplicationLegalInformationPath>(),
                MakeServiceCommandMeta<CommandId::Refresh, &LocationResolverService::Refresh>(),

                /* 5.0.0- */
                MakeServiceCommandMeta<CommandId::RedirectApplicationProgramPath, &LocationResolverService::RedirectApplicationProgramPath, FirmwareVersion_500>(),
                MakeServiceCommandMeta<CommandId::ClearApplicationRedirection, &LocationResolverService::ClearApplicationRedirection>(),
                MakeServiceCommandMeta<CommandId::EraseProgramRedirection, &LocationResolverService::EraseProgramRedirection>(),
                MakeServiceCommandMeta<CommandId::EraseApplicationControlRedirection, &LocationResolverService::EraseApplicationControlRedirection>(),
                MakeServiceCommandMeta<CommandId::EraseApplicationHtmlDocumentRedirection, &LocationResolverService::EraseApplicationHtmlDocumentRedirection>(),
                MakeServiceCommandMeta<CommandId::EraseApplicationLegalInformationRedirection, &LocationResolverService::EraseApplicationLegalInformationRedirection>(),
            };
    };

    class LocationResolverInterface : public LocationResolverService {
        public:
            u32 unk_0x60 = 1;

            LocationResolverInterface(FsStorageId storage_id);

            Result RefreshImpl();
            
        protected:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) override;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) override;
            virtual Result Refresh() override;
    };

    class HostLocationResolverInterface : public LocationResolverService {
        public:
            HostLocationResolverInterface(FsStorageId storage_id);

        protected:
            virtual Result ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) override;
            virtual Result ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid) override;
            virtual Result Refresh() override;
    };

}