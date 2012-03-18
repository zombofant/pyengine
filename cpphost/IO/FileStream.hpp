/**********************************************************************
File name: FileStream.hpp
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
#ifndef _PYUNI_IO_FILESTREAM_H
#define _PYUNI_IO_FILESTREAM_H

#include "Stream.hpp"

namespace PyUni {
    
static inline int checkFD(int fd) {
    if (fd == -1) {
        raiseLastOSError();
    }
    return fd;
}


enum WriteMode {
    WM_IGNORE = 0,
    WM_OVERWRITE = 1,
    WM_APPEND = 2
};

enum OpenMode {
    OM_READ = 0,
    OM_WRITE = 1,
    OM_BOTH = 2
};

enum ShareMode {
    SM_EXCLUSIVE = 0,
    SM_ALLOW_READ = 1,
    SM_ALLOW_WRITE = 2,
    SM_ALLOW_BOTH = 3,
    SM_DONT_CARE = 4
};

class FileError: public StreamError {
    public:
        FileError(const std::string message):
            StreamError(message) {};
        FileError(const char *message):
            StreamError(message) {};
        virtual ~FileError() throw() {};
};

class FDStream: public Stream {
    public:
        FDStream(int fd, bool ownsFD = true);
        virtual ~FDStream() throw();
    protected:
        int _fd;
        bool _ownsFD;
    public:
        virtual void flush();
        virtual sizeuint read(void *data, const sizeuint length);
        virtual sizeuint seek(const int whence, const sizeint offset);
        virtual const sizeuint size() const;
        virtual const sizeuint tell() const;
        virtual sizeuint write(const void *data, const sizeuint length);
};

/**
 * Opens a stream to access a file. This is how the OpenMode:s and
 * WriteMode:s map to open(2) modes.
 * 
 * OpenMode     WriteMode       open flags
 * OM_READ      any             O_RDONLY
 * OM_WRITE     WM_IGNORE       O_WRONLY|O_TRUNC|O_CREAT
 * OM_WRITE     WM_OVERWRITE    O_WRONLY|O_TRUNC|O_CREAT
 * OM_WRITE     WM_APPEND       O_WRONLY|O_APPEND|O_CREAT
 * OM_BOTH      WM_IGNORE       O_RDWR|O_TRUNC|O_CREAT
 * OM_BOTH      WM_OVERWRITE    O_RDWR|O_TRUNC|O_CREAT
 * OM_BOTH      WM_APPEND       O_RDWR|O_APPEND|O_CREAT
 * 
 * As you can see from the table, for FileStream WM_IGNORE and 
 * WM_OVERWRITE are equivalent. However, this is only a coincidence and
 * should be treated as undocumented feature and not relied upon.
 * 
 * All considerations which can be found in the open(2) man page apply.
 */
class FileStream: public FDStream {
    public:
        FileStream(const std::string fileName, const OpenMode openMode,
            const WriteMode writeMode = WM_IGNORE,
            const ShareMode shareMode = SM_DONT_CARE);
    private:
        const OpenMode _openMode;
        bool _seekable;
    public:
        virtual bool isReadable() const;
        virtual bool isSeekable() const;
        virtual bool isWritable() const;
};

}

#endif
