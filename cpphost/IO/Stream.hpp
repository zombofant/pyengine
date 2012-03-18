/**********************************************************************
File name: Stream.hpp
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
#ifndef _PYUNI_IO_STREAM_H
#define _PYUNI_IO_STREAM_H

#include <cstdint>
#include "Misc/Int.hpp"
#include "Misc/Exception.hpp"
#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace IO {
    
/**
 * Just that we can be sure these two are equivalent.
 */
static_assert((sizeof(ssize_t) == sizeof(off_t)) && (sizeof(ssize_t) == sizeof(sizeint)), "Sizes of ssize_t and off_t differ.");

class StreamError: public Exception {
    public:
        StreamError(const std::string message):
            Exception(message) {};
        StreamError(const char *message):
            Exception(message) {};
        virtual ~StreamError() throw() {};
};

class StreamNotSupportedError: public StreamError {
    public:
        StreamNotSupportedError(const std::string message):
            StreamError(message) {};
        StreamNotSupportedError(const char *message):
            StreamError(message) {};
        virtual ~StreamNotSupportedError() throw() {};
};

class StreamReadError: public StreamError {
    public:
        StreamReadError(const std::string message):
            StreamError(message) {};
        StreamReadError(const char *message):
            StreamError(message) {};
        virtual ~StreamReadError() throw() {};
};

class StreamWriteError: public StreamError {
    public:
        StreamWriteError(const std::string message):
            StreamError(message) {};
        StreamWriteError(const char *message):
            StreamError(message) {};
        virtual ~StreamWriteError() throw() {};
};

/** 
 * Class to replace the std::[io]?[f]?stream classes. The rationale for
 * that can be... uh... requested at some of the developers.
 * 
 * These streams are binary only. If you want to write human-readable
 * data, you are supposed to format it to a string and use the write
 * string operator.
 * 
 * However, we do not provide a >> operator for strings, as any 
 * behaviour we could supply would not be satisfactory for all purposes.
 * If you want to read strings, you should use the supplied methods on
 * your own.
 */
class Stream {
    public:
        /**
         * Make sure the stream is synchronized with any low-level,
         * hardware or file system primitives. The meaning of this call
         * is dependent on the actual stream type.
         * 
         * A flush call should generally be made before switching
         * from reading to writing and vice versa or when itsk neccessary
         * to be sure that data is actually stored (or sent, if its a 
         * network stream for example).
         */
        virtual void flush();
    
        /**
         * Attempts to read length bytes from the stream and stores 
         * these in data. Returns the amount of bytes read. This might
         * be less than length if an error occured. The contents of 
         * data behind the last position where data was written to 
         * should be considered as uninitialized.
         * 
         * This may throw an exception if it is not possible to read 
         * from the stream.
         * 
         * You may check for reading ability by calling isReadable.
         */
        sizeuint read(char *data, const sizeuint length);
        
        virtual sizeuint read(void *data, const sizeuint length);
        
        /**
         * Change the read/write pointer position of the stream. For
         * documentation of whence and offset, see lseek(2).
         * 
         * This may throw an exception if it is not possible to seek
         * in the stream.
         * 
         * You may check for seeking ability by calling isSeekable.
         */
        virtual sizeuint seek(const int whence, const sizeint offset);
        
        /**
         * Returns the size of the stream in bytes. This may throw an
         * exception if seeking is not supported. You can check for
         * seek support by calling isSeekable.
         * 
         * A similar effect can be achived by calling seek(SEEK_END, 0),
         * but this does not change the current reading/writing 
         * position.
         */
        virtual const sizeuint size() const;
        
        /**
         * This has the same effect as seek(SEEK_CUR, 0), except that it
         * should also work on non-seekable streams and constant 
         * references. 
         * 
         * Return value: absolute position of the read/write pointer 
         * from the beginning of the stream.
         * 
         * You may check for seeking/telling ability by calling 
         * isSeekable. If a stream is seekable, it must be able to tell
         * the current position.
         * 
         * tell must never throw an exception. At may return 0 if 
         * telling the position/bytecount is not supported.
         */
        virtual const sizeuint tell() const;
        
        /**
         * Write length bytes read from data to the stream. Returns the
         * amount of bytes actually written which might be less than
         * length in case of an error.
         * 
         * This may throw an exception if its not possible to write to
         * the stream.
         * 
         * You may check for writing ability by calling isWritable.
         */
        virtual sizeuint write(const void *data, const sizeuint length);
        sizeuint write(const char *data, const sizeuint length);
    protected:
        void raiseReadError(const sizeuint read, const sizeuint required);
        void raiseWriteError(const sizeuint written, const sizeuint required);
        template <class _T> _T readInt();
        template <class _T> void writeInt(const _T value);
    public:
        int8 readInt8();
        int16 readInt16();
        int32 readInt32();
        int64 readInt64();
        std::string readString(const sizeuint length);
        uint8 readUInt8();
        uint16 readUInt16();
        uint32 readUInt32();
        uint64 readUInt64();
        
        /** 
         * a convenience function to overcome the need to write an \n
         * or \r or both to a char buffer. Writes the current OS:s line
         * ending to the stream.
         */
        void writeEndl();
        void writeInt8(const int8 value);
        void writeInt16(const int16 value);
        void writeInt32(const int32 value);
        void writeInt64(const int64 value);
        void writeString(const std::string &value);
        void writeString(const char* value);
        void writeUInt8(const uint8 value);
        void writeUInt16(const uint16 value);
        void writeUInt32(const uint32 value);
        void writeUInt64(const uint64 value);
    public:
        virtual bool isReadable() const = 0;
        virtual bool isSeekable() const = 0;
        virtual bool isWritable() const = 0;
};

typedef boost::shared_ptr<Stream> StreamHandle;

#include "includes/StreamOperators.hpp.inc"

}
}

#endif
