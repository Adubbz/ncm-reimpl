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

    enum RedirectionFlags {
        RedirectionFlags_None        = (0 << 0),
        RedirectionFlags_Application = (1 << 0),
    };

    class LocationRedirection  : public util::IntrusiveListBaseNode<LocationRedirection> {
        NON_COPYABLE(LocationRedirection);
        NON_MOVEABLE(LocationRedirection);

        public:
            ncm::TitleId title_id;
            Path path;
            u32 flags;

            LocationRedirection(ncm::TitleId title_id, const Path& path, u32 flags) :
                title_id(title_id), path(path), flags(flags) {
            }
    };

    class LocationRedirector {
        NON_COPYABLE(LocationRedirector);
        NON_MOVEABLE(LocationRedirector);
        private:
            sts::util::IntrusiveListBaseTraits<LocationRedirection>::ListType redirection_list;
        public:
            LocationRedirector();

            bool FindRedirection(Path *out, ncm::TitleId title_id);
            void SetRedirection(ncm::TitleId title_id, const Path &path, u32 flags = RedirectionFlags_None);
            void SetRedirectionFlags(ncm::TitleId title_id, u32 flags);
            void EraseRedirection(ncm::TitleId title_id);
            void ClearRedirections(u32 flags = RedirectionFlags_None);
    };

    class RegisteredLocationRedirection {
        public:
            Path path;
            ncm::TitleId title_id;

            RegisteredLocationRedirection(const Path path, const ncm::TitleId title_id) : path(path), title_id(title_id) {
            }
    };

    class RegisteredLocationRedirector {
        static constexpr size_t MaxRedirections = 16;

        private:
            std::array<std::optional<RegisteredLocationRedirection>, MaxRedirections> redirections;
        public:
            RegisteredLocationRedirector();

            bool FindRedirection(Path *out, ncm::TitleId title_id);
            bool SetRedirection(ncm::TitleId title_id, const Path& path);
            void EraseRedirection(ncm::TitleId title_id);
            void ClearRedirections();
    }; 

    class AddOnContentRedirection {
        public:
            ncm::StorageId storage_id;
            ncm::TitleId title_id;

            AddOnContentRedirection(ncm::StorageId storage_id, ncm::TitleId title_id) : storage_id(storage_id), title_id(title_id) {
            }
    };

    class AddOnContentRedirector {
        static constexpr size_t MaxRedirections = 128;

        private:
            std::array<std::optional<AddOnContentRedirection>, MaxRedirections> redirections;
        public:
            bool FindRedirection(ncm::StorageId *out, ncm::TitleId title_id);
            Result SetRedirection(ncm::TitleId title_id, ncm::StorageId storage_id);
            void ClearRedirections();
    };

}