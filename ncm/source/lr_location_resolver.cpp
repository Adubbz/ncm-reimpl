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

#include "lr_location_resolver.hpp"

LocationResolverBase::LocationResolverBase(FsStorageId storage_id) :
    storage_id(storage_id)
{
    // TODO: CreateContentMetaDatabase
    // TODO: CreateContentStorage
}

Result LocationResolverBase::RedirectProgramPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->program_location_list, tid, path.pointer, false);
    return 0;
}

Result LocationResolverBase::ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid)
{
    char path[FS_MAX_PATH] = {0};

    if (!Registration::ResolvePath(&this->app_control_location_list, path, tid))
        return 0x1008;

    memcpy(out.pointer, path, FS_MAX_PATH);
    return 0;
}

Result LocationResolverBase::ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid)
{
    char path[FS_MAX_PATH] = {0};

    if (!Registration::ResolvePath(&this->html_docs_location_list, path, tid))
        return 0xC08;

    memcpy(out.pointer, path, FS_MAX_PATH);
    return 0;
}

Result LocationResolverBase::RedirectApplicationControlPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->app_control_location_list, tid, path.pointer, true);
    return 0;
}

Result LocationResolverBase::RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->html_docs_location_list, tid, path.pointer, true);
    return 0;
}

Result LocationResolverBase::ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid)
{
    char path[FS_MAX_PATH] = {0};

    if (!Registration::ResolvePath(&this->legal_info_location_list, path, tid))
        return 0x1208;

    memcpy(out.pointer, path, FS_MAX_PATH);
    return 0;
}

Result LocationResolverBase::RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->legal_info_location_list, tid, path.pointer, true);
    return 0;
}

Result LocationResolverBase::RedirectApplicationProgramPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->program_location_list, tid, path.pointer, true);
    return 0;
}

LocationResolver::LocationResolver(FsStorageId storage_id) :
    LocationResolverBase(storage_id)
{
}

Result LocationResolver::RefreshImpl()
{
    return 0xF601;
}

Result LocationResolver::ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result LocationResolver::ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result LocationResolver::Refresh()
{
    return 0xF601;
}

Result LocationResolver::ClearApplicationRedirection()
{
    return 0xF601;
}

Result LocationResolver::EraseProgramRedirection(u64 tid)
{
    return 0xF601;
}

Result LocationResolver::EraseApplicationControlRedirection(u64 tid)
{
    return 0xF601;
}

Result LocationResolver::EraseApplicationHtmlDocumentRedirection(u64 tid)
{
    return 0xF601;
}

Result LocationResolver::EraseApplicationLegalInformationRedirection(u64 tid)
{
    return 0xF601;
}

HostLocationResolver::HostLocationResolver(FsStorageId storage_id) :
    LocationResolverBase(storage_id)
{
}

Result HostLocationResolver::ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0x608; // Unsupported operation for context
}

Result HostLocationResolver::Refresh()
{
    return 0xF601;
}

Result HostLocationResolver::ClearApplicationRedirection()
{
    return 0xF601;
}

Result HostLocationResolver::EraseProgramRedirection(u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::EraseApplicationControlRedirection(u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::EraseApplicationHtmlDocumentRedirection(u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::EraseApplicationLegalInformationRedirection(u64 tid)
{
    return 0xF601;
}