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
    program_location_list.last_entry = reinterpret_cast<LocationListEntry*>(&program_location_list.last_entry);
    program_location_list.first_entry = reinterpret_cast<LocationListEntry*>(&program_location_list.last_entry);
    app_control_location_list.last_entry = reinterpret_cast<LocationListEntry*>(&app_control_location_list.last_entry);
    app_control_location_list.first_entry = reinterpret_cast<LocationListEntry*>(&app_control_location_list.last_entry);
    html_docs_location_list.last_entry = reinterpret_cast<LocationListEntry*>(&html_docs_location_list.last_entry);
    html_docs_location_list.first_entry = reinterpret_cast<LocationListEntry*>(&html_docs_location_list.last_entry);
    legal_info_location_lost.last_entry = reinterpret_cast<LocationListEntry*>(&legal_info_location_lost.last_entry);
    legal_info_location_lost.first_entry = reinterpret_cast<LocationListEntry*>(&legal_info_location_lost.last_entry);

    // TODO: CreateContentMetaDatabase
    // TODO: CreateContentStorage
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

Result LocationResolver::RedirectProgramPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result LocationResolver::ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result LocationResolver::ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result LocationResolver::ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result LocationResolver::RedirectApplicationControlPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result LocationResolver::RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result LocationResolver::ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result LocationResolver::RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result LocationResolver::Refresh()
{
    return 0xF601;
}

Result LocationResolver::RedirectProgramPath2(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result LocationResolver::Refresh2()
{
    return 0xF601;
}

Result LocationResolver::DeleteProgramPath(u64 tid)
{
    return 0xF601;
}

Result LocationResolver::DeleteApplicationControlPath(u64 tid)
{
    return 0xF601;
}

Result LocationResolver::DeleteApplicationHtmlDocumentPath(u64 tid)
{
    return 0xF601;
}

Result LocationResolver::DeleteApplicationLegalInformationPath(u64 tid)
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

Result HostLocationResolver::RedirectProgramPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result HostLocationResolver::ResolveApplicationControlPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::ResolveDataPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::RedirectApplicationControlPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result HostLocationResolver::RedirectApplicationHtmlDocumentPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result HostLocationResolver::ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::RedirectApplicationLegalInformationPath(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result HostLocationResolver::Refresh()
{
    return 0xF601;
}

Result HostLocationResolver::RedirectProgramPath2(u64 tid, InPointer<const char> path)
{
    return 0xF601;
}

Result HostLocationResolver::Refresh2()
{
    return 0xF601;
}

Result HostLocationResolver::DeleteProgramPath(u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::DeleteApplicationControlPath(u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::DeleteApplicationHtmlDocumentPath(u64 tid)
{
    return 0xF601;
}

Result HostLocationResolver::DeleteApplicationLegalInformationPath(u64 tid)
{
    return 0xF601;
}