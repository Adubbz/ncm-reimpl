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

enum LrAocLrCmd : u32
{
    LrAocLr_Cmd_ResolveAddOnContentPath = 0,
    LrAocLr_Cmd_RegisterAddOnContentStorage = 1,
    LrAocLr_Cmd_UnregisterAllAddOnContentPath = 2,
};

class AddOnContentLocationResolverInterface : public IServiceObject
{
    private:
        Result ResolveAddOnContentPath(OutPointerWithClientSize<char> out, u64 tid);
        Result RegisterAddOnContentStorage(FsStorageId storage_id, u64 tid);
        Result UnregisterAllAddOnContentPath();

    public:
        DEFINE_SERVICE_DISPATCH_TABLE 
        {
            /* 1.0.0- */
            MakeServiceCommandMeta<LrAocLr_Cmd_ResolveAddOnContentPath, &AddOnContentLocationResolverInterface::ResolveAddOnContentPath>(),
            MakeServiceCommandMeta<LrAocLr_Cmd_RegisterAddOnContentStorage, &AddOnContentLocationResolverInterface::RegisterAddOnContentStorage>(),
            MakeServiceCommandMeta<LrAocLr_Cmd_UnregisterAllAddOnContentPath, &AddOnContentLocationResolverInterface::UnregisterAllAddOnContentPath>(),
        };
};