/**********************************************************************
File name: FileStream.cpp
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

#include "FileStream.hpp"
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include "Log.hpp"

namespace PyUni {
namespace IO {
    
/* PyUni::IO::FDStream */

FDStream::FDStream(int fd, bool ownsFD):
    _fd(fd),
    _ownsFD(ownsFD)
{
    
}

FDStream::~FDStream() throw() {
    if (_ownsFD) {
        close(_fd);
        _fd = 0;
    }
}

void FDStream::flush() {
    fsync(_fd);
}

sizeuint FDStream::read(void *data, const sizeuint length) {
    sizeint readBytes = ::read(_fd, data, length);
    if (readBytes == -1) {
        raiseLastOSError();
    }
    return readBytes;
}

sizeuint FDStream::seek(const int whence, const sizeint offset) {
    sizeint soughtOffset = lseek(_fd, offset, whence);
    if (offset == -1) {
        raiseLastOSError();
    }
    return soughtOffset;
}

const sizeuint FDStream::size() const {
    const sizeint pos = tell();
    const sizeint fsize = lseek(_fd, 0, SEEK_END);
    lseek(_fd, pos, SEEK_SET);
    return fsize;
}

const sizeuint FDStream::tell() const {
    return lseek(_fd, 0, SEEK_CUR);
}

sizeuint FDStream::write(const void *data, const sizeuint length) {
    sizeint writtenBytes = ::write(_fd, data, length);
    if (writtenBytes == -1) {
        raiseLastOSError();
    }
    return writtenBytes;
}

/* PyUni::IO::FileStream */

// note that throwing the exception on a failed open is done in checkFD
FileStream::FileStream(const std::string fileName, 
    const OpenMode openMode, const WriteMode writeMode,
    const ShareMode shareMode):
    FDStream::FDStream(
        checkFD(open(
            fileName.c_str(),
            (openMode==OM_BOTH?O_RDWR:(openMode==OM_READ?O_RDONLY:O_WRONLY)) |
            (((openMode!=OM_READ) && ((writeMode==WM_OVERWRITE) || (writeMode==WM_IGNORE)))?O_CREAT:O_APPEND),
            (S_IRWXU | S_IRWXG | S_IRWXO) & (~(S_IXUSR | S_IXGRP | S_IXOTH))
        )), true),
    _openMode(openMode)
{
    if (shareMode != SM_DONT_CARE) {
        // log << ML_WARNING << "File stream for `" << fileName << "' opened with a ShareMode nonequal to SM_DONT_CARE. This is ignored." << submit;
    }
    struct stat fileStat;
    fileStat.st_mode = 0;
    fstat(_fd, &fileStat);
    _seekable = !(S_ISFIFO(fileStat.st_mode) || S_ISSOCK(fileStat.st_mode) || S_ISCHR(fileStat.st_mode));
}

bool FileStream::isReadable() const {
    return (_openMode != OM_WRITE);
}

bool FileStream::isSeekable() const {
    return _seekable;
}

bool FileStream::isWritable() const {
    return (_openMode != OM_READ);
}
    
}
}
