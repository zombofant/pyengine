/**********************************************************************
File name: FileStream.cpp
This file is part of: Pythonic Engine

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

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include "FileStream.hpp"

#include <cerrno>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace PyEngine {

/* PyEngine::FDStream */

FDStream::FDStream(int fd, bool owns_fd):
    _fd(fd),
    _owns_fd(owns_fd)
{

}

FDStream::~FDStream() throw() {
    close();
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

sizeuint FDStream::size() const {
    const sizeint pos = tell();
    const sizeint fsize = lseek(_fd, 0, SEEK_END);
    lseek(_fd, pos, SEEK_SET);
    return fsize;
}

sizeuint FDStream::tell() const {
    return lseek(_fd, 0, SEEK_CUR);
}

sizeuint FDStream::write(const void *data, const sizeuint length) {
    sizeint writtenBytes = ::write(_fd, data, length);
    if (writtenBytes == -1) {
        raiseLastOSError();
    }
    return writtenBytes;
}

void FDStream::close() {
    if (_owns_fd) {
        ::close(_fd);
    }
    _fd = 0;
}

/* PyEngine::FileStream */

// note that throwing the exception on a failed open is done in checkFD
FileStream::FileStream(const std::string &filename,
                       const OpenMode openmode,
                       const WriteMode writemode,
                       const ShareMode sharemode):
    FDStream::FDStream(
        checkFD(open_file_with_modes(
                    filename,
                    openmode,
                    writemode,
                    sharemode)),
        true),
    _openmode(openmode)
{
    if (sharemode != SM_DONT_CARE) {
        // log << ML_WARNING << "File stream for `" << fileName << "' opened with a ShareMode nonequal to SM_DONT_CARE. This is ignored." << submit;
    }
    struct stat fileStat;
    fileStat.st_mode = 0;
    fstat(_fd, &fileStat);
    _seekable = !(S_ISFIFO(fileStat.st_mode)
                  || S_ISSOCK(fileStat.st_mode)
                  || S_ISCHR(fileStat.st_mode));
}

bool FileStream::isReadable() const {
    return (_openmode != OM_WRITE);
}

bool FileStream::isSeekable() const {
    return _seekable;
}

bool FileStream::isWritable() const {
    return (_openmode != OM_READ);
}

/* free functions */


int open_file_with_modes(const std::string &filename,
                         const OpenMode openmode,
                         const WriteMode writemode,
                         const ShareMode sharemode)
{
    int flags = 0;

    switch (openmode) {
    case OM_READ:
    {
        flags = O_RDONLY;
        break;
    }
    case OM_WRITE:
    case OM_BOTH:
    {
        flags = O_CREAT;
        switch (writemode) {
        case WM_IGNORE:
        case WM_OVERWRITE:
        {
            flags |= O_TRUNC;
        }
        case WM_APPEND:
        {
            flags |= O_APPEND;
        }
        }

        if (openmode == OM_BOTH) {
            flags |= O_RDWR;
        } else {
            flags |= O_WRONLY;
        }

        break;
    }
    }

    return open(
        filename.c_str(),
        flags,
        (S_IRWXU | S_IRWXG | S_IRWXO) & (~(S_IXUSR | S_IXGRP | S_IXOTH)));
}

}
