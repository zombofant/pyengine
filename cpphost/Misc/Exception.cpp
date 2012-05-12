/**********************************************************************
File name: Exception.cpp
This file is part of: Pythonic Universe

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#include "Exception.hpp"

#include <execinfo.h>
#include <boost/format.hpp>

namespace PyUni {
    
/* PyUni::Utils::Exception */

Exception::Exception(const std::string message):
    _message(message),
    _tracebackLength(16)
{
    _traceback = (void**)calloc(_tracebackLength, sizeof(void*));
    _tracebackLength = backtrace(_traceback, 16);
}

Exception::Exception(const char *message):
    _message(message),
    _tracebackLength(16)
{
    _traceback = (void**)calloc(_tracebackLength, sizeof(void*));
    _tracebackLength = backtrace(_traceback, 16);
}

/* PyUni::Utils::IndexError */

IndexError::IndexError(const int given, const int min, const int max):
    Exception::Exception((boost::format("Index (%d) out of range (%d..%d).") % given % min % max).str())
{
    
}

IndexError::IndexError(const int given):
    Exception::Exception((boost::format("Index (%d) out of range.") % given).str())
{
    
}

/* PyUni::Utils::ExternalError */
ExternalError::ExternalError(const char *libraryName):
    Exception::Exception((boost::format("External error in %s.") % libraryName).str())
{
    
}

ExternalError::ExternalError(const char *libraryName, const char *externalMsg):
    Exception::Exception((boost::format("External error in %s: %s") % libraryName % externalMsg).str())
{
    
}

/* PyUni::Utils::OSError */
OSError::OSError(const std::string message):
    Exception::Exception((boost::format("OS error: %s.") % message).str())
{
    
}

/* free functions */

std::string getErrorName(const int errorNumber) {
    
    switch (errorNumber) {
        case EACCES:
        {
            return "Permission denied";
        }
        default:
            return (boost::format("Unkown error code %d.") % errorNumber).str();
    }
}

void raiseLastOSError() {
    throw OSError(getErrorName(errno));
}

void raiseLastOSError(const std::string prefix) {
    throw OSError((boost::format("%s: %s") % prefix % getErrorName(errno)).str());
}

std::ostream &operator<<(std::ostream &stream, Exception &exception)  {
    unsigned int count;
    void * const* traceback = exception.traceback(&count);
    stream << "Exception occured: " << typeid(exception).name() << std::endl;
    stream << "Message: " << exception.what() << std::endl;
    stream << "Traceback: (most recent call first)" << std::endl;
    char **tracebackSymbols = backtrace_symbols(traceback, count);
    for (unsigned int i = 0; i < count; i++) {
        stream << tracebackSymbols[i] << std::endl;
    }
    free(tracebackSymbols);
    return stream;
}

}
