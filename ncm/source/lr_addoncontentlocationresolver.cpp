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

#include "lr_addoncontentlocationresolver.hpp"

namespace sts::lr {

    Result AddOnContentLocationResolverInterface::ResolveAddOnContentPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result AddOnContentLocationResolverInterface::RegisterAddOnContentStorage(ncm::StorageId storage_id, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result AddOnContentLocationResolverInterface::UnregisterAllAddOnContentPath() {
        return ResultKernelConnectionClosed;
    }

}