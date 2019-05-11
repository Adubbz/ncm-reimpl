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
    return ResultSuccess;
}

Result LocationResolverBase::ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid)
{
    char path[FS_MAX_PATH] = {0};

    if (!Registration::ResolvePath(&this->app_control_location_list, path, tid))
        return ResultLrControlNotFound;

    memcpy(out.pointer, path, FS_MAX_PATH-1);
    return ResultSuccess;
}

Result LocationResolverBase::ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid)
{
    char path[FS_MAX_PATH] = {0};

    if (!Registration::ResolvePath(&this->html_docs_location_list, path, tid))
        return ResultLrHtmlDocumentNotFound;

    memcpy(out.pointer, path, FS_MAX_PATH-1);
    return ResultSuccess;
}

Result LocationResolverBase::RedirectApplicationControlPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->app_control_location_list, tid, path.pointer, true);
    return ResultSuccess;
}

Result LocationResolverBase::RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->html_docs_location_list, tid, path.pointer, true);
    return ResultSuccess;
}

Result LocationResolverBase::ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid)
{
    char path[FS_MAX_PATH] = {0};

    if (!Registration::ResolvePath(&this->legal_info_location_list, path, tid))
        return ResultLrLegalInformationNotFound;

    memcpy(out.pointer, path, FS_MAX_PATH-1);
    return ResultSuccess;
}

Result LocationResolverBase::RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->legal_info_location_list, tid, path.pointer, true);
    return ResultSuccess;
}

Result LocationResolverBase::RedirectApplicationProgramPath(u64 tid, InPointer<const char> path)
{
    Registration::RedirectPath(&this->program_location_list, tid, path.pointer, true);
    return ResultSuccess;
}

Result LocationResolverBase::ClearApplicationRedirection()
{
    Registration::EraseRedirectionWithMask(&this->program_location_list, 1);
    Registration::EraseRedirectionWithMask(&this->app_control_location_list, 1);
    Registration::EraseRedirectionWithMask(&this->html_docs_location_list, 1);
    Registration::EraseRedirectionWithMask(&this->legal_info_location_list, 1);
    return ResultSuccess;
}

Result LocationResolverBase::EraseProgramRedirection(u64 tid)
{
    Registration::EraseRedirectionWithTid(&this->program_location_list, tid);
    return ResultSuccess;
}

Result LocationResolverBase::EraseApplicationControlRedirection(u64 tid)
{
    Registration::EraseRedirectionWithTid(&this->app_control_location_list, tid);
    return ResultSuccess;
}

Result LocationResolverBase::EraseApplicationHtmlDocumentRedirection(u64 tid)
{
    Registration::EraseRedirectionWithTid(&this->html_docs_location_list, tid);
    return ResultSuccess;
}

Result LocationResolverBase::EraseApplicationLegalInformationRedirection(u64 tid)
{
    Registration::EraseRedirectionWithTid(&this->legal_info_location_list, tid);
    return ResultSuccess;
}

LocationResolver::LocationResolver(FsStorageId storage_id) :
    LocationResolverBase(storage_id)
{
}

Result LocationResolver::RefreshImpl()
{
    return ResultKernelConnectionClosed;
}

Result LocationResolver::ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return ResultKernelConnectionClosed;
}

Result LocationResolver::ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return ResultKernelConnectionClosed;
}

Result LocationResolver::Refresh()
{
    return ResultKernelConnectionClosed;
}

HostLocationResolver::HostLocationResolver(FsStorageId storage_id) :
    LocationResolverBase(storage_id)
{
}

Result HostLocationResolver::ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return ResultKernelConnectionClosed;
}

Result HostLocationResolver::ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return ResultKernelConnectionClosed; // Unsupported operation for context
}

Result HostLocationResolver::Refresh()
{
    return ResultKernelConnectionClosed;
}