/**********************************************************************
File name: Utils.hpp
This file is part of: Pythonic Engine

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#ifndef _PYE_VFS_UTILS_H
#define _PYE_VFS_UTILS_H

#include <string>

namespace PyEngine {

std::string absolutify(const std::string &path);
std::string basename(const std::string &path);
std::string dirname(const std::string &path);
std::string join(const std::initializer_list<std::string> &segments);
std::string normalize_vfs_path(const std::string &path);
std::pair<std::string, std::string> splitext(const std::string &path);
void validate_vfs_path(const std::string &path);

}

#endif
