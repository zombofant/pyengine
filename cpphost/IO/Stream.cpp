/**********************************************************************
File name: Stream.cpp
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
#include "Stream.hpp"

#include <cstring>
#include <typeinfo>
#include <limits>

#include <boost/format.hpp>

namespace PyUni {

/* PyUni::Stream */

void Stream::flush()
{
    
}

sizeuint Stream::read(void *data, const sizeuint length)
{
    throw StreamNotSupportedError((boost::format("%s does not support reading.") % typeid(this).name()).str());
}

sizeuint Stream::read(char *data, const sizeuint length)
{
    return read((void*)data, length);
}

sizeuint Stream::seek(const int whence, const sizeint offset)
{
    raiseSeekNotSupportedError();
    return 0;
}

const sizeuint Stream::size() const
{
    raiseSeekNotSupportedError();
    return 0;
}

const sizeuint Stream::tell() const
{
    return 0;
}

sizeuint Stream::write(const void *data, const sizeuint length)
{
    throw StreamNotSupportedError((boost::format("%s does not support writing.") % typeid(this).name()).str());
}

sizeuint Stream::write(const char *data, const sizeuint length)
{
    return write((const void*)data, length);
}

void Stream::raiseSeekNotSupportedError() const
{
    throw StreamNotSupportedError((boost::format("%s does not support seeking.") % typeid(this).name()).str());
}

void Stream::raiseReadError(const sizeuint read, const sizeuint required) const
{
    throw StreamReadError((boost::format("Stream read error. Read %d bytes, wanted %d.") % read % required).str());
}

void Stream::raiseWriteError(const sizeuint written, const sizeuint required) const
{
    throw StreamReadError((boost::format("Stream write error. Wrote %d bytes, wanted %d.") % written % required).str());
}

template <class _T> _T Stream::readInt()
{
    _T result;
    const sizeuint readBytes = read(&result, sizeof(_T));
    if (readBytes < sizeof(_T)) {
        raiseReadError(readBytes, sizeof(_T));
    }
    return result;
}

template <class _T> void Stream::writeInt(const _T value)
{
    const sizeuint writtenBytes = write(&value, sizeof(_T));
    if (writtenBytes < sizeof(_T)) {
        raiseWriteError(writtenBytes, sizeof(_T));
    }
}

void Stream::readBytes(void *data, const sizeuint length)
{
    const sizeuint readBytes = read(data, length);
    if (readBytes < length) {
        raiseReadError(readBytes, length);
    }
}

int8 Stream::readInt8()
{
    return readInt<int8>();
}

int16 Stream::readInt16()
{
    return readInt<int16>();
}

int32 Stream::readInt32()
{
    return readInt<int32>();
}

int64 Stream::readInt64()
{
    return readInt<int64>();
}

std::string Stream::readString(const sizeuint length)
{
    void *buffer = malloc(length);
    try {
        readBytes(buffer, length);
    } catch (...) {
        free(buffer);
        throw;
    }
    std::string result((char*)buffer);
    free(buffer);
    return result;
}

uint8 Stream::readUInt8()
{
    return readInt<uint8>();
}

uint16 Stream::readUInt16()
{
    return readInt<uint16>();
}

uint32 Stream::readUInt32()
{
    return readInt<uint32>();
}

uint64 Stream::readUInt64()
{
    return readInt<uint64>();
}

void Stream::writeEndl()
{
    #if defined (__linux__)
        static const int length = 1;
        static const char lineEnding[length+1] = "\n";
    #else
        #if defined (__win32__)
            static const int length = 2;
            static const char lineEnding[length+1] = "\r\n";
        #else
            #if defined (__APPLE__)
                static const int length = 1;
                static const char lineEnding[length+1] = "\r";
            #else
                static_assert(false, "Could not detect operating system.");
            #endif
        #endif
    #endif
    write(lineEnding, length);
}

void Stream::writeInt8(const int8 value)
{
    return writeInt<int8>(value);
}

void Stream::writeInt16(const int16 value)
{
    return writeInt<int16>(value);
}

void Stream::writeInt32(const int32 value)
{
    return writeInt<int32>(value);
}

void Stream::writeInt64(const int64 value)
{
    return writeInt<int64>(value);
}

void Stream::writeString(const std::string &value)
{
    
    // TODO: Replace this by a proper <limits> query
    if (value.size() >= 4294967296) {
        throw StreamError((boost::format("String is too long (%d).") % value.size()).str());
    }
    const uint32 length = value.size();
    const sizeuint writtenBytes = write(value.c_str(), length);
    if (writtenBytes < length) {
        raiseWriteError(writtenBytes, length);
    }
}

void Stream::writeString(const char *value)
{
    const sizeuint len = strlen(value);
    const sizeuint writtenBytes = write(value, len);
    if (writtenBytes < len) {
        raiseWriteError(writtenBytes, len);
    }
}

void Stream::writeUInt8(const uint8 value)
{
    return writeInt<uint8>(value);
}

void Stream::writeUInt16(const uint16 value)
{
    return writeInt<uint16>(value);
}

void Stream::writeUInt32(const uint32 value)
{
    return writeInt<uint32>(value);
}

void Stream::writeUInt64(const uint64 value)
{
    return writeInt<uint64>(value);
}

#include "includes/StreamOperators.cpp.inc"

}
