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

#include "lr_manager_service.hpp"

namespace sts::lr {

    std::shared_ptr<LocationResolverService>* LocationResolverManagerService::GetLocationResolverPtr(ncm::StorageId storage_id) {
        int highest_active_index = -1;

        for (unsigned int i = 0; i < 5; i++) {
            auto entry = this->entries[i];

            // If there already is a location resolver, return the address of that
            if (entry.active) {
                if (entry.storage_id == storage_id) {
                    return &this->location_resolvers[i];
                } else {
                    highest_active_index = i;
                }
            }
        }

        if (highest_active_index == 4) {
            // There are no free entries, and there are no existing ones with this storage id either
            std::abort();
        }

        return &this->location_resolvers[highest_active_index + 1];
    }

    Result LocationResolverManagerService::OpenLocationResolver(Out<std::shared_ptr<LocationResolverService>> out, ncm::StorageId storage_id) {
        std::scoped_lock lk{this->mutex};
        std::shared_ptr<LocationResolverService> resolver = nullptr;

        ON_SCOPE_EXIT {
            out.SetValue(std::move(resolver));
        };

        // If there already is a location resolver, return that
        for (unsigned int i = 0; i < 5; i++) {
            auto entry = this->entries[i];

            if (entry.active && entry.storage_id == storage_id) {
                resolver = this->location_resolvers[i];
                return ResultSuccess;
            }
        }

        if (storage_id == ncm::StorageId::Host) {
            auto location_resolver = std::make_shared<HostLocationResolverInterface>(storage_id);
            auto* lr_ptr = this->GetLocationResolverPtr(storage_id);
            *lr_ptr = location_resolver;
            resolver = location_resolver;
        } else {
            auto location_resolver = std::make_shared<LocationResolverInterface>(storage_id);
            R_TRY(location_resolver->RefreshImpl());

            auto* lr_ptr = this->GetLocationResolverPtr(storage_id);
            *lr_ptr = location_resolver;
            resolver = location_resolver;
        }

        // Once again search for a usable location resolver. This seems kinda unnecessary but hey,
        // Ninty does it so we will too.
        for (unsigned int i = 0; i < 5; i++) {
            auto entry = this->entries[i];

            if (entry.active && entry.storage_id == storage_id) {
                resolver = this->location_resolvers[i];
                return ResultSuccess;
            }
        }

        return ResultSuccess;
    }

    Result LocationResolverManagerService::OpenRegisteredLocationResolver(Out<std::shared_ptr<RegisteredLocationResolverInterface>> out) {
        return ResultKernelConnectionClosed;
    }

    Result LocationResolverManagerService::RefreshLocationResolver(ncm::StorageId storage_id) {
        return ResultKernelConnectionClosed;
    }

    Result LocationResolverManagerService::OpenAddOnContentLocationResolver(Out<std::shared_ptr<AddOnContentLocationResolverInterface>> out) {
        return ResultKernelConnectionClosed;
    }

}