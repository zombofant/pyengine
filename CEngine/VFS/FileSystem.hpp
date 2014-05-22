/**********************************************************************
File name: FileSystem.hpp
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
#ifndef _PYE_VFS_FILESYSTEM_H
#define _PYE_VFS_FILESYSTEM_H

#include <map>
#include <list>
#include <vector>
#include <memory>

#include "CEngine/IO/Stream.hpp"
#include "CEngine/IO/FileStream.hpp"

#include "Common.hpp"

namespace PyEngine {

class FileSystem
{
public:
    FileSystem();
    ~FileSystem();

private:
    typedef std::pair<std::string, MountPtr> MountItem;
    typedef std::vector<MountItem> MountList;

private:
    std::map<MountPriority, MountList, std::greater<MountPriority>> _mounts;

protected:
    std::pair<MountPriority, MountList::iterator> find_mount(
        const Mount* mount);

    void iter_file_mounts(
        const std::string &path,
        const std::function<bool (Mount*, const std::string&)> &handler);
    void sort_mount_list(MountList &list);

public:
    void listdir(const std::string &path,
                 std::vector<std::string> &items);

    void mount(
        const std::string &mount_point,
        MountPtr &&mount,
        MountPriority priority);

    StreamHandle open(
        const std::string &path,
        const OpenMode open_mode,
        const WriteMode write_mode = WM_IGNORE);

    void stat(const std::string &path,
              VFSStat &stat);
};

}

#endif
