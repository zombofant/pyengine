/**********************************************************************
File name: Mount.hpp
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
#ifndef _PYE_VFS_MOUNT_H
#define _PYE_VFS_MOUNT_H

#include <memory>
#include <vector>

#include "CEngine/IO/Stream.hpp"
#include "CEngine/IO/FileStream.hpp"

#include "Common.hpp"

namespace PyEngine {

class Mount
{
public:
    Mount();
    virtual ~Mount();

public:
    virtual std::string get_real_path(const std::string &local_path);

    virtual void listdir(
        const std::string &local_path,
        std::vector<std::string> &items) = 0;

    virtual StreamHandle open(
        const std::string &local_path,
        const OpenMode open_mode,
        const WriteMode write_mode) = 0;

    virtual void stat(const std::string &local_path, VFSStat &stat) = 0;
};

class MountDirectory: public Mount
{
public:
    MountDirectory(
        const std::string &fs_path,
        bool read_only = true);

private:
    const std::string _root;
    bool _read_only;

private:
    static std::string canonicalize_file_name(const char *name);

public:
    std::string get_real_path(const std::string &local_path) override;
    void listdir(
        const std::string &local_path,
        std::vector<std::string> &items) override;
    StreamHandle open(
        const std::string &local_path,
        const OpenMode open_mode,
        const WriteMode write_mode) override;
    void stat(const std::string &local_path, VFSStat &stat) override;
};

typedef std::unique_ptr<Mount> MountPtr;

}

#endif
