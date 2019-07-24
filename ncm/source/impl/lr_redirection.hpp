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

#include "../lr_types.hpp"

namespace sts::lr::impl {

    class LocationRedirection  : public util::IntrusiveListBaseNode<LocationRedirection> {
        NON_COPYABLE(LocationRedirection);
        NON_MOVEABLE(LocationRedirection);

        private:
            ncm::TitleId title_id;
            Path path;
            u32 flags;
    };

    class LocationRedirector {
        NON_COPYABLE(LocationRedirector);
        NON_MOVEABLE(LocationRedirector);
        private:
            using RedirectionList = sts::util::IntrusiveListBaseTraits<LocationRedirection>::ListType;

            RedirectionList program_list;
            RedirectionList app_control_list;
            RedirectionList html_docs_list;
            RedirectionList legal_info_list;
        public:
            LocationRedirector();

            bool FindRedirection(Path *out, ncm::TitleId title_id);
            void SetRedirection(ncm::TitleId title_id, const Path &path, u32 flags = 0);
            void SetRedirectionFlags(ncm::TitleId title_id, u32 flags);
            void EraseRedirection(ncm::TitleId title_id);
            void ClearRedirections(u32 flags = 0);
    };

}