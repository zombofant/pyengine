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
