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

#include "impl/ncm_content_manager.hpp"
#include "lr_redirectonlylocationresolver.hpp"

namespace sts::lr {

    Result RedirectOnlyLocationResolverInterface::ResolveProgramPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        Path path;
        
        if (this->program_redirector.FindRedirection(&path, tid)) {
            *out.pointer = path;
            return ResultSuccess;
        }

        return ResultLrProgramNotFound;
    }

    Result RedirectOnlyLocationResolverInterface::ResolveDataPath(OutPointerWithServerSize<Path, 0x1> out, ncm::TitleId tid) {
        return ResultLrDataNotFound;
    }

    Result RedirectOnlyLocationResolverInterface::Refresh() {
        this->program_redirector.ClearRedirections();
        this->debug_program_redirector.ClearRedirections();
        this->app_control_redirector.ClearRedirections();
        this->html_docs_redirector.ClearRedirections();
        this->legal_info_redirector.ClearRedirections();
        return ResultSuccess;
    }

}