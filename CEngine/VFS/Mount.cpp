#include "Mount.hpp"

#include <stdlib.h>

#include "Utils.hpp"

namespace PyEngine {

/* PyEngine::Mount */

Mount::Mount()
{

}

std::string Mount::get_real_path(const std::string &local_path)
{
    return "";
}

/* PyEngine::MountDirectory */

MountDirectory::MountDirectory(
        const std::string &fs_path,
        bool read_only):
    _root(canonicalize_file_name(fs_path.c_str())),
    _read_only(read_only)
{

}

std::string MountDirectory::canonicalize_file_name(
    const char *name)
{
    // FIXME: portability
    char *canonical = realpath(name, nullptr);
    std::string result;
    try {
        result = canonical;
    } catch (...) {
        free(canonical);
        throw;
    }
    free(canonical);

    return result;
}

std::string MountDirectory::get_real_path(const std::string &local_path)
{
    return join({_root, local_path});
}

}
