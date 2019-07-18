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

#include "lr_registered_location_resolver.hpp"

namespace sts::lr {

    Result RegisteredLocationResolverService::ResolveProgramPath(OutPointerWithClientSize<char> out, u64 tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::RegisterProgramPath(u64 tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::UnregisterProgramPath(u64 tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::RedirectProgramPath(u64 tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::ResolveHtmlDocumentPath(OutPointerWithClientSize<char> out, u64 tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::RegisterHtmlDocumentPath(u64 tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::UnregisterHtmlDocumentPath(u64 tid) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::RedirectHtmlDocumentPath(u64 tid, InPointer<const char> path) {
        return ResultKernelConnectionClosed;
    }

    Result RegisteredLocationResolverService::Refresh() {
        return ResultKernelConnectionClosed;
    }

}