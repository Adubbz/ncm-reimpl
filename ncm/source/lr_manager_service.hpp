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
        protected:
            enum class CommandId {
                OpenLocationResolver = 0,
                OpenRegisteredLocationResolver = 1,
                RefreshLocationResolver = 2,
                OpenAddOnContentLocationResolver = 3,
            };
        private:
            struct LocationResolverEntry {
                bool active;
                ncm::StorageId storage_id;
            };

            LocationResolverEntry entries[5] = {0};
            std::shared_ptr<LocationResolverBase> location_resolvers[5] = {0};
            HosMutex mutex;
        private:
            std::shared_ptr<LocationResolverBase>* GetLocationResolverPtr(ncm::StorageId storage_id);
        public:
            /* Actual commands. */
            virtual Result OpenLocationResolver(Out<std::shared_ptr<LocationResolverBase>> out, ncm::StorageId storage_id);
            virtual Result OpenRegisteredLocationResolver(Out<std::shared_ptr<RegisteredLocationResolverService>> out);
            virtual Result RefreshLocationResolver(ncm::StorageId storage_id);
            virtual Result OpenAddOnContentLocationResolver(Out<std::shared_ptr<AddOnContentLocationResolverService>> out);
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(LocationResolverManagerService, OpenLocationResolver),
                MAKE_SERVICE_COMMAND_META(LocationResolverManagerService, OpenRegisteredLocationResolver),
                MAKE_SERVICE_COMMAND_META(LocationResolverManagerService, RefreshLocationResolver),
                MAKE_SERVICE_COMMAND_META(LocationResolverManagerService, OpenAddOnContentLocationResolver, FirmwareVersion_200),
            };
    };

}