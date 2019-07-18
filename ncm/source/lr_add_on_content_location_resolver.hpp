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

namespace sts::lr {

    class AddOnContentLocationResolverService : public IServiceObject {
        protected:
            enum class CommandId {
                ResolveAddOnContentPath = 0,
                RegisterAddOnContentStorage = 1,
                UnregisterAllAddOnContentPath = 2,
            };
        public:
            virtual Result ResolveAddOnContentPath(OutPointerWithClientSize<char> out, ncm::TitleId tid);
            virtual Result RegisterAddOnContentStorage(ncm::StorageId storage_id, ncm::TitleId tid);
            virtual Result UnregisterAllAddOnContentPath();
        public:
            DEFINE_SERVICE_DISPATCH_TABLE {
                MAKE_SERVICE_COMMAND_META(AddOnContentLocationResolverService, ResolveAddOnContentPath),
                MAKE_SERVICE_COMMAND_META(AddOnContentLocationResolverService, RegisterAddOnContentStorage),
                MAKE_SERVICE_COMMAND_META(AddOnContentLocationResolverService, UnregisterAllAddOnContentPath),
            };
    };

}