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

#include "lr_contentlocationresolver.hpp"

namespace sts::lr {

    Result ContentLocationResolverInterface::ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectProgramPath(ncm::TitleId tid, InPointer<const char> path) {
        program_redirector.SetRedirection(tid, path.pointer);
        return ResultSuccess;
    }

    Result ContentLocationResolverInterface::ResolveApplicationControlPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ResolveApplicationHtmlDocumentPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ResolveDataPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationControlPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationHtmlDocumentPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ResolveApplicationLegalInformationPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationLegalInformationPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::Refresh() {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationProgramPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ClearApplicationRedirection() {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::EraseProgramRedirection(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::EraseApplicationControlRedirection(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::EraseApplicationHtmlDocumentRedirection(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::EraseApplicationLegalInformationRedirection(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

}