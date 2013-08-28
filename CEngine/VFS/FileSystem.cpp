#include "FileSystem.hpp"

#include "Errors.hpp"
#include "Mount.hpp"
#include "Utils.hpp"

namespace PyEngine {

/* PyEngine::FileSystem */

FileSystem::FileSystem():
    _mounts()
{

}

void FileSystem::iter_file_mounts(
    const std::string &path,
    const std::function<bool (Mount*, const std::string&)> &handler)
{
    for (auto &prio_list: _mounts)
    {
        MountList &list = prio_list.second;
        for (auto &path_mount: list)
        {
            std::string &mountpoint = path_mount.first;
            if (path.compare(0, mountpoint.size(), mountpoint) != 0) {
                continue;
            }
            bool finish = handler(
                path_mount.second.get(),
                path.substr(mountpoint.size()+1));
            if (finish) {
                return;
            }
        }
    }
}

void FileSystem::sort_mount_list(FileSystem::MountList &list)
{
    std::sort(list.begin(), list.end(),
        [](const MountItem &a, const MountItem &b){
            return a.first.size() < b.first.size();
        });
}

void FileSystem::listdir(
    const std::string &path, std::vector<std::string> &items)
{
    bool found = false;
    bool had_permission_denied = false;

    iter_file_mounts(
        path,
        [&found, &items, &had_permission_denied](
            Mount *mount,
            const std::string &local_path) mutable -> bool
        {
            try {
                mount->listdir(local_path, items);
            } catch (const VFSPermissionDeniedError &err) {
                had_permission_denied = true;
                return false;
            } catch (const VFSIOError &err) {
                return false;
            }
            found = true;
            return true;
        });

    if (!found) {
        if (had_permission_denied) {
            throw VFSPermissionDeniedError(path);
        } else {
            throw VFSFileNotFoundError(path);
        }
    }
}

void FileSystem::mount(
        const std::string &mount_point,
        MountPtr &&mount,
        MountPriority priority)
{
    std::string path = normalize_vfs_path(mount_point);
    validate_vfs_path(path);

    _mounts[priority].push_back(std::make_pair(path, std::move(mount)));
    sort_mount_list(_mounts[priority]);
}

StreamHandle FileSystem::open(
    const std::string &path,
    const OpenMode open_mode,
    const WriteMode write_mode)
{
    bool had_permission_denied = false;
    StreamHandle result;

    iter_file_mounts(
        path,
        [&result, &had_permission_denied, &open_mode, &write_mode](
            Mount *mount,
            const std::string &local_path) mutable -> bool
        {
            try {
                result = mount->open(local_path, open_mode, write_mode);
            } catch (const VFSPermissionDeniedError &err) {
                had_permission_denied = true;
            } catch (const VFSIOError &err) {

            }
            if (result) {
                return true;
            }
            return false;
        });

    if (!result) {
        if (had_permission_denied) {
            throw VFSPermissionDeniedError(path);
        } else {
            throw VFSFileNotFoundError(path);
        }
    }
    return result;
}

void FileSystem::stat(const std::string &path, VFSStat &stat)
{
    bool had_permission_denied = false;
    bool success = false;

    iter_file_mounts(
        path,
        [&stat, &success, &had_permission_denied](
            Mount *mount,
            const std::string &local_path) mutable -> bool
        {
            try {
                mount->stat(local_path, stat);
                success = true;
                return true;
            } catch (const VFSPermissionDeniedError &err) {
                had_permission_denied = true;
                return false;
            } catch (const VFSIOError &err) {
                return false;
            }
            return false;
        });

    if (!success) {
        if (had_permission_denied) {
            throw VFSPermissionDeniedError(path);
        } else {
            throw VFSFileNotFoundError(path);
        }
    }
}

}
