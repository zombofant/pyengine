/**********************************************************************
File name: Exception.hpp
This file is part of: tt3d — Freeform transport simulation

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

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _PYUNI_UTILS_EXCEPTION_H
#define _PYUNI_UTILS_EXCEPTION_H

#include <exception>
#include <string>

namespace PyUni {
 
class Exception: public std::exception {
    public:
        Exception(const std::string message);
        Exception(const char *message);
        virtual ~Exception() throw() {free(_traceback);};
    private:
        const std::string _message;
        unsigned int _tracebackLength;
        void **_traceback;
    public:
        virtual const char *what() const throw() {
            return _message.c_str();
        }
        
        void* const*traceback(unsigned int *count) const { *count = _tracebackLength; return _traceback; }
};

class IndexError: public Exception {
    public:
        IndexError(const int given, const int min, const int max);
        IndexError(const int given);
        virtual ~IndexError() throw() {};
};

class ExternalError: public Exception {
    public:
        ExternalError(const char *libraryName);
        ExternalError(const char *libraryName, const char *externalMsg);
        virtual ~ExternalError() throw() {};
};

class OSError: public Exception {
    public:
        OSError(const std::string message);
        virtual ~OSError() throw() {};
};

void raiseLastOSError();
void raiseLastOSError(const std::string prefix);

std::ostream &operator<<(std::ostream &stream, Exception &exception);


}

#endif
