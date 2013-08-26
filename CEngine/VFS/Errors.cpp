#include "Errors.hpp"

namespace PyEngine {

/* PyEngine::VFSIOError */

VFSIOError::VFSIOError(const std::string &what_arg):
    Exception(what_arg)
{

}

VFSIOError::VFSIOError(const char *what_arg):
    Exception(what_arg)
{

}

/* PyEngine::VFSPermissionDeniedError */

VFSPermissionDeniedError::VFSPermissionDeniedError(
        const std::string &what_arg):
    VFSIOError("Permission denied: "+what_arg)
{

}

/* PyEngine::VFSFileNotFoundError */

VFSFileNotFoundError::VFSFileNotFoundError(
        const std::string &what_arg):
    VFSIOError("File not found: "+what_arg)
{

}

}
