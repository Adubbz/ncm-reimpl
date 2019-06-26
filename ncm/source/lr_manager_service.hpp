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
#include "lr_location_resolver.hpp"
#include "lr_registered_location_resolver.hpp"
#include "lr_add_on_content_location_resolver.hpp"

namespace sts::lr {

    class LocationResolverManagerService final : public IServiceObject {
        private:
            struct LocationResolverEntry {
                bool active;
                FsStorageId storage_id;
            };

            LocationResolverEntry entries[5] = {0};
            std::shared_ptr<LocationResolverService> location_resolvers[5] = {0};
            HosMutex mutex;

        private:
            std::shared_ptr<LocationResolverService>* GetLocationResolverPtr(FsStorageId storage_id);

        private:
            enum class CommandId {
                OpenLocationResolver = 0,
                OpenRegisteredLocationResolver = 1,
                RefreshLocationResolver = 2,
                OpenAddOnContentLocationResolver = 3,
            };

            /* Actual commands. */
            Result OpenLocationResolver(Out<std::shared_ptr<LocationResolverService>> out, FsStorageId storage_id);
            Result OpenRegisteredLocationResolver(Out<std::shared_ptr<RegisteredLocationResolverInterface>> out);
            Result RefreshLocationResolver(FsStorageId storage_id);
            Result OpenAddOnContentLocationResolver(Out<std::shared_ptr<AddOnContentLocationResolverInterface>> out);

        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                /* 1.0.0- */
                MakeServiceCommandMeta<CommandId::OpenLocationResolver, &LocationResolverManagerService::OpenLocationResolver>(),
                MakeServiceCommandMeta<CommandId::OpenRegisteredLocationResolver, &LocationResolverManagerService::OpenRegisteredLocationResolver>(),
                MakeServiceCommandMeta<CommandId::RefreshLocationResolver, &LocationResolverManagerService::RefreshLocationResolver>(),
            
                /* 2.0.0- */
                MakeServiceCommandMeta<CommandId::OpenAddOnContentLocationResolver, &LocationResolverManagerService::OpenAddOnContentLocationResolver, FirmwareVersion_200>(),
            };
    };

}