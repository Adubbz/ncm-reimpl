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

#include "lr_registeredlocationresolver.hpp"

namespace sts::lr {

    Result RegisteredLocationResolverInterface::ResolveProgramPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::RegisterProgramPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::UnregisterProgramPath(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::RedirectProgramPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::ResolveHtmlDocumentPath(OutPointerWithClientSize<char> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::RegisterHtmlDocumentPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::UnregisterHtmlDocumentPath(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::RedirectHtmlDocumentPath(ncm::TitleId tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverInterface::Refresh() {
        return ResultKernelConnectionClosed;
    }

}