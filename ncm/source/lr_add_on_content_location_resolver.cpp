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

#include "lr_add_on_content_location_resolver.hpp"

Result AddOnContentLocationResolverInterface::ResolveAddOnContentPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return ResultKernelConnectionClosed;
}

Result AddOnContentLocationResolverInterface::RegisterAddOnContentStorage(FsStorageId storage_id, u64 tid)
{
    return ResultKernelConnectionClosed;
}

Result AddOnContentLocationResolverInterface::UnregisterAllAddOnContentPath()
{
    return ResultKernelConnectionClosed;
}