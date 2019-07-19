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

namespace sts::lr {

    LocationResolverBase::LocationResolverBase(ncm::StorageId storage_id) :
        storage_id(storage_id) {
        // TODO: CreateContentMetaDatabase
        // TODO: CreateContentStorage
    }

    Result LocationResolverBase::RedirectProgramPath(ncm::TitleId tid, InPointer<const char> path) {
        reg::RedirectPath(&this->program_location_list, tid, path.pointer, false);
        return ResultSuccess;
    }

    Result LocationResolverBase::ResolveApplicationControlPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        char path[FS_MAX_PATH] = {0};

        if (!reg::ResolvePath(&this->app_control_location_list, path, tid))
            return ResultLrControlNotFound;

        memcpy(out.pointer, path, FS_MAX_PATH-1);
        return ResultSuccess;
    }

    Result LocationResolverBase::ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        char path[FS_MAX_PATH] = {0};

        if (!reg::ResolvePath(&this->html_docs_location_list, path, tid))
            return ResultLrHtmlDocumentNotFound;

        memcpy(out.pointer, path, FS_MAX_PATH-1);
        return ResultSuccess;
    }

    Result LocationResolverBase::RedirectApplicationControlPath(ncm::TitleId tid, InPointer<const char> path) {
        reg::RedirectPath(&this->app_control_location_list, tid, path.pointer, true);
        return ResultSuccess;
    }

    Result LocationResolverBase::RedirectApplicationHtmlDocumentPath(ncm::TitleId tid, InPointer<const char> path) {
        reg::RedirectPath(&this->html_docs_location_list, tid, path.pointer, true);
        return ResultSuccess;
    }

    Result LocationResolverBase::ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        char path[FS_MAX_PATH] = {0};

        if (!reg::ResolvePath(&this->legal_info_location_list, path, tid))
            return ResultLrLegalInformationNotFound;

        memcpy(out.pointer, path, FS_MAX_PATH-1);
        return ResultSuccess;
    }

    Result LocationResolverBase::RedirectApplicationLegalInformationPath(ncm::TitleId tid, InPointer<const char> path) {
        reg::RedirectPath(&this->legal_info_location_list, tid, path.pointer, true);
        return ResultSuccess;
    }

    Result LocationResolverBase::RedirectApplicationProgramPath(ncm::TitleId tid, InPointer<const char> path) {
        reg::RedirectPath(&this->program_location_list, tid, path.pointer, true);
        return ResultSuccess;
    }

    Result LocationResolverBase::ClearApplicationRedirection() {
        reg::EraseRedirectionWithMask(&this->program_location_list, 1);
        reg::EraseRedirectionWithMask(&this->app_control_location_list, 1);
        reg::EraseRedirectionWithMask(&this->html_docs_location_list, 1);
        reg::EraseRedirectionWithMask(&this->legal_info_location_list, 1);
        return ResultSuccess;
    }

    Result LocationResolverBase::EraseProgramRedirection(ncm::TitleId tid) {
        reg::EraseRedirectionWithTid(&this->program_location_list, tid);
        return ResultSuccess;
    }

    Result LocationResolverBase::EraseApplicationControlRedirection(ncm::TitleId tid) {
        reg::EraseRedirectionWithTid(&this->app_control_location_list, tid);
        return ResultSuccess;
    }

    Result LocationResolverBase::EraseApplicationHtmlDocumentRedirection(ncm::TitleId tid) {
        reg::EraseRedirectionWithTid(&this->html_docs_location_list, tid);
        return ResultSuccess;
    }

    Result LocationResolverBase::EraseApplicationLegalInformationRedirection(ncm::TitleId tid) {
        reg::EraseRedirectionWithTid(&this->legal_info_location_list, tid);
        return ResultSuccess;
    }

    Result LocationResolverBase::ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed; // Overridden
    }

    Result LocationResolverBase::ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;// Overridden
    }

    Result LocationResolverBase::Refresh() {
        return ResultKernelConnectionClosed; // Overridden
    }

    LocationResolverInterface::LocationResolverInterface(ncm::StorageId storage_id) :
        LocationResolverBase(storage_id) {
    }

    Result LocationResolverInterface::RefreshImpl() {
        return ResultKernelConnectionClosed;
    }

    Result LocationResolverInterface::ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result LocationResolverInterface::ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result LocationResolverInterface::Refresh() {
        return ResultKernelConnectionClosed;
    }

    HostLocationResolverInterface::HostLocationResolverInterface(ncm::StorageId storage_id) :
        LocationResolverBase(storage_id) {
    }

    Result HostLocationResolverInterface::ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result HostLocationResolverInterface::ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed; // Unsupported operation for context
    }

    Result HostLocationResolverInterface::Refresh() {
        return ResultKernelConnectionClosed;
    }

}