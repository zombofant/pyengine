#ifndef _PYE_VFS_UTILS_H
#define _PYE_VFS_UTILS_H

#include <string>

namespace PyEngine {

std::string absolutify(const std::string &path);
std::string join(const std::initializer_list<std::string> &segments);
std::string normalize_vfs_path(const std::string &path);
std::pair<std::string, std::string> splitext(const std::string &path);
void validate_vfs_path(const std::string &path);

}

#endif
