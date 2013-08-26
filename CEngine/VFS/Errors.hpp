#ifndef _PYE_VFS_ERRORS_H
#define _PYE_VFS_ERRORS_H

#include "CEngine/Misc/Exception.hpp"

namespace PyEngine {

class VFSIOError: public Exception
{
public:
    VFSIOError(const std::string &what_arg);
    VFSIOError(const char *what_arg);

};

class VFSPermissionDeniedError: public VFSIOError
{
public:
    VFSPermissionDeniedError(const std::string &path);

};

class VFSFileNotFoundError: public VFSIOError
{
public:
    VFSFileNotFoundError(const std::string &path);

};

}

#endif
