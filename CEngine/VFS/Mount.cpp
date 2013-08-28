#include "Mount.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "Errors.hpp"
#include "Utils.hpp"

namespace PyEngine {

/* PyEngine::Mount */

Mount::Mount()
{

}

Mount::~Mount()
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

void MountDirectory::listdir(
    const std::string &local_path,
    std::vector<std::string> &items)
{
    const std::string full_dir_path = join({_root, local_path});
    // FIXME: portability
    DIR *dir = opendir(full_dir_path.c_str());
    if (dir == nullptr) {
        switch (errno) {
        case EACCES:
            throw VFSPermissionDeniedError(local_path);
        case ENOENT:
            throw VFSFileNotFoundError(local_path);
        default:
            throw VFSIOError(std::string("IO error: ") + strerror(errno));
        }
    }

    dirent *ent = nullptr;
    while ((ent = readdir(dir)) != nullptr) {
        items.push_back(std::string(ent->d_name));
    }
    closedir(dir);
}

StreamHandle MountDirectory::open(
    const std::string &local_path,
    const OpenMode open_mode,
    const WriteMode write_mode)
{
    if (_read_only && (open_mode != OM_READ)) {
        throw VFSPermissionDeniedError(local_path);
    }

    const std::string full_dir_path = join({_root, local_path});
    try {
        return StreamHandle(new FileStream(
            full_dir_path,
            open_mode,
            write_mode));
    } catch (const OSError &err) {
        if (errno == EACCES) {
            throw VFSPermissionDeniedError(local_path);
        } else if (errno == ENOENT) {
            throw VFSFileNotFoundError(local_path);
        } else {
            throw VFSIOError(err.what());
        }
    }
    return nullptr;
}

void MountDirectory::stat(const std::string &local_path, VFSStat &stat)
{
    const std::string full_dir_path = join({_root, local_path});
    // FIXME: portability

    std::cout << full_dir_path << std::endl;

    struct stat os_buf;
    int result = ::stat(full_dir_path.c_str(), &os_buf);
    if (result != 0) {
        std::cout << "failure: " << errno << " " << strerror(errno)
                  << std::endl;
        switch (errno) {
        case EACCES:
            throw VFSPermissionDeniedError(local_path);
        case ENOENT:
            throw VFSFileNotFoundError(local_path);
        default:
            throw VFSIOError(std::string("IO error: ") + strerror(errno));
        }
    }

    stat.mount = this;
    stat.mode = 0;

    if (S_ISREG(os_buf.st_mode)) {
        stat.mode |= VSM_REGULAR;
    }
    if (S_ISDIR(os_buf.st_mode)) {
        stat.mode |= VSM_DIRECTORY;
    }

    stat.mtime = std::chrono::system_clock::from_time_t(
        os_buf.st_mtime);
    stat.size = os_buf.st_size;
}

}
