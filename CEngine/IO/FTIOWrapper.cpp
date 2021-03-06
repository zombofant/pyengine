/**********************************************************************
File name: FTIOWrapper.cpp
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
#include "FTIOWrapper.hpp"

#include "IO.hpp"

namespace PyEngine {

using namespace std;

inline Stream *extractStream(FT_Stream stream)
{
    return ((KeepAlive<StreamHandle>*)stream->descriptor.pointer)->handle.get();
}

unsigned long FT_read(FT_Stream ftStream, unsigned long offset,
    unsigned char* buffer, unsigned long count)
{
    Stream *stream = extractStream(ftStream);
    stream->seek(SEEK_CUR, offset);
    if (count == 0)
        return 0;
    stream->read((char*)buffer, count);
    return count;
}

void FT_close(FT_Stream stream)
{
    delete (KeepAlive<StreamHandle>*)stream->descriptor.pointer;
}

FT_Stream fromIStream(StreamHandle input)
{
    Stream *stream = input.get();
    FT_Stream result = new FT_StreamRec();
    result->base = 0;
    result->size = stream->size();
    result->pos = stream->tell();
    result->descriptor.pointer = (void*)(new KeepAlive<StreamHandle>(input));
    result->pathname.pointer = 0;
    result->read = &FT_read;
    result->close = &FT_close;
    return result;
}

FT_Open_Args *openAsStream(StreamHandle stream)
{
    FT_Open_Args *result = new FT_Open_Args();
    result->flags = FT_OPEN_STREAM;
    result->memory_base = 0;
    result->memory_size = 0;
    result->pathname = 0;
    result->stream = fromIStream(stream);
    result->driver = 0;
    result->num_params = 0;
    result->params = 0;
    return result;
}

}
