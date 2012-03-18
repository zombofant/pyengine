/**********************************************************************
File name: PNGIOWrapper.cpp
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
#include "PNGIOWrapper.hpp"

namespace PyUni {

inline Stream *extractStream(png_structp png_ptr)
{
    return (Stream*)png_get_io_ptr(png_ptr);
}

void iostream_write_data(png_structp png_ptr,
    png_bytep data, png_size_t length)
{
    Stream *stream = extractStream(png_ptr);
    stream->write(data, length);
}

void iostream_flush_data(png_structp png_ptr)
{
    Stream *stream = extractStream(png_ptr);
    stream->flush();
}

void iostream_read_data(png_structp png_ptr,
    png_bytep data, png_size_t length)
{
    Stream *stream = extractStream(png_ptr);
    // this will throw if we run out of data
    stream->read((char*)data, length);
}

void png_init_io(png_structp data, Stream *stream)
{
    png_set_read_fn(data, stream, &iostream_read_data);
}

}
