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
    bool file_readable(
        const std::string &path);

    bool file_writable(
        const std::string &path);

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
