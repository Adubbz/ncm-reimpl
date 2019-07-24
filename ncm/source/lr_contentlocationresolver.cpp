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

    Result ContentLocationResolverInterface::ResolveProgramPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        Path path;

        if (this->program_redirector.FindRedirection(&path, tid)) {
            *out.pointer = path;
            return ResultSuccess;
        }

        ncm::ContentId program_content_id;

        R_TRY_CATCH(this->content_meta_database->GetLatestProgram(&program_content_id, tid)) {
            R_CATCH(ResultNcmContentMetaNotFound) {
                return ResultLrProgramNotFound;
            }
        } R_END_TRY_CATCH;
        
        R_ASSERT(this->content_storage->GetPath(OutPointerWithServerSize<Path, 0x1>(path.path, FS_MAX_PATH-1), program_content_id));
        /* Copying ensures path is null-terminated. */
        *out.pointer = path;
        return ResultSuccess;
    }

    Result ContentLocationResolverInterface::RedirectProgramPath(ncm::TitleId tid, InPointer<const Path> path) {
        this->program_redirector.SetRedirection(tid, *path.pointer);
        return ResultSuccess;
    }

    Result ContentLocationResolverInterface::ResolveApplicationControlPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ResolveApplicationHtmlDocumentPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ResolveDataPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationControlPath(ncm::TitleId tid, InPointer<const Path> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationHtmlDocumentPath(ncm::TitleId tid, InPointer<const Path> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::ResolveApplicationLegalInformationPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationLegalInformationPath(ncm::TitleId tid, InPointer<const Path> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::Refresh() {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectApplicationProgramPath(ncm::TitleId tid, InPointer<const Path> path) {
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

    Result ContentLocationResolverInterface::ResolveProgramPathForDebug(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::RedirectProgramPathForDebug(ncm::TitleId tid, InPointer<const Path> path) {
        return ResultKernelConnectionClosed;    
    }

    Result ContentLocationResolverInterface::RedirectApplicationProgramPathForDebug(ncm::TitleId tid, InPointer<const Path> path) {
        return ResultKernelConnectionClosed;
    }

    Result ContentLocationResolverInterface::EraseProgramRedirectionForDebug(ncm::TitleId tid) {
        return ResultKernelConnectionClosed;
    }

}