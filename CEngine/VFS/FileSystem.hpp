#ifndef _PYE_VFS_FILESYSTEM_H
#define _PYE_VFS_FILESYSTEM_H

#include <map>
#include <list>
#include <vector>
#include <memory>

#include "CEngine/IO/Stream.hpp"
#include "CEngine/IO/FileStream.hpp"

namespace PyEngine {

class Mount;
typedef std::unique_ptr<Mount> MountPtr;

enum MountPriority
{
    PracticallyInexistant = -3,
    Discriminated = -2,
    Fallback = -1,
    FileSystem = 0,
    Important = 1,
    Override = 2,
    Penetrant = 3
};

class FileSystem
{
public:
    FileSystem();
    ~FileSystem();

private:
    typedef std::vector<std::pair<std::string, MountPtr>> MountList;

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
    void mount(
        const std::string &mount_point,
        MountPtr &&mount,
        MountPriority priority);

    bool file_readable(
        const std::string &path);

    bool file_writable(
        const std::string &path);

    StreamHandle open(
        const std::string &path,
        const OpenMode open_mode,
        const WriteMode write_mode = WM_IGNORE);

    template <typename OutputIterator>
    void listdir(const std::string &path);
};

}

#endif
