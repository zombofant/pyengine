#ifndef _PYE_VFS_MOUNT_H
#define _PYE_VFS_MOUNT_H

#include <memory>
#include <vector>

#include "CEngine/IO/Stream.hpp"
#include "CEngine/IO/FileStream.hpp"

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
};

typedef std::unique_ptr<Mount> MountPtr;

}

#endif
