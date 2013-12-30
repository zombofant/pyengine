/**********************************************************************
File name: Common.hpp
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
#ifndef _PYE_VFS_COMMON_H
#define _PYE_VFS_COMMON_H

#include <chrono>
#include <memory>

namespace PyEngine {

class Mount;
typedef std::unique_ptr<Mount> MountPtr;

enum class MountPriority
{
    PracticallyInexistant = -3,
    Discriminated = -2,
    Fallback = -1,
    FileSystem = 0,
    Important = 1,
    Override = 2,
    Penetrant = 3
};

enum VFSStatMode
{
    VSM_UNUSED =        0x0000001,
    VSM_WRITABLE =      0x0000002,
    VSM_READABLE =      0x0000004,
    VSM_DIRECTORY =     0x0040000,
    VSM_REGULAR =       0x0100000
};

struct VFSStat
{
    Mount *mount;
    uint32_t mode;
    size_t size;
    std::chrono::system_clock::time_point mtime;
};

}

#endif
