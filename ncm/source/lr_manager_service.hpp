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

enum LrManagerCmd : u32
{
    Lrm_Cmd_OpenLocationResolver = 0,
    Lrm_Cmd_OpenRegisteredLocationResolver = 1,
    Lrm_Cmd_RefreshLocationResolver = 2,
    Lrm_Cmd_OpenAddOnContentLocationResolver = 3,
};

class LocationResolverManagerService final : public IServiceObject
{
    public:
        struct LocationResolverEntry
        {
            bool active;
            FsStorageId storage_id;
        };

        LocationResolverEntry entries[5] = {0};
        std::shared_ptr<LocationResolverBase> location_resolvers[5] = {0};
        HosMutex mutex;

    private:
        std::shared_ptr<LocationResolverBase>* GetFreeLocationResolverPtr(FsStorageId storage_id);

        /* Actual commands. */
        Result OpenLocationResolver(Out<std::shared_ptr<LocationResolverBase>> out, FsStorageId storage_id);
        Result OpenRegisteredLocationResolver(Out<std::shared_ptr<RegisteredLocationResolverInterface>> out);
        Result RefreshLocationResolver(FsStorageId storage_id);
        Result OpenAddOnContentLocationResolver(Out<std::shared_ptr<AddOnContentLocationResolverInterface>> out);

    public:
        DEFINE_SERVICE_DISPATCH_TABLE
        {
            /* 1.0.0- */
            MakeServiceCommandMeta<Lrm_Cmd_OpenLocationResolver, &LocationResolverManagerService::OpenLocationResolver>(),
            MakeServiceCommandMeta<Lrm_Cmd_OpenRegisteredLocationResolver, &LocationResolverManagerService::OpenRegisteredLocationResolver>(),
            MakeServiceCommandMeta<Lrm_Cmd_RefreshLocationResolver, &LocationResolverManagerService::RefreshLocationResolver>(),
        
            /* 2.0.0- */
            MakeServiceCommandMeta<Lrm_Cmd_OpenAddOnContentLocationResolver, &LocationResolverManagerService::OpenAddOnContentLocationResolver, FirmwareVersion_200>(),
        };
};