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

using namespace std;

void iostream_write_data(png_structp png_ptr,
    png_bytep data, png_size_t length);
void iostream_flush_data(png_structp png_ptr);

void iostream_read_data(png_structp png_ptr,
    png_bytep data, png_size_t length)
{
    istream *input = (istream*)png_get_io_ptr(png_ptr);
    // this will throw if we run out of data
    input->read((char*)data, length);
}

}

using namespace std;
using namespace PyUni;

void png_init_io(png_structp data, istream &input)
{
    png_set_read_fn(data, &input, &iostream_read_data);
}

/*void png_init_io(png_structp data, ostream &output)
{
    png_set_write_fn(data, &output, &iostream_write_data,
        &iostream_flush_data);
}

void png_init_io(png_structp data, iostream &io)
{
    png_set_read_fn(data, &io, &iostream_read_data);
    png_set_write_fn(data, &io, &iostream_write_data,
        &iostream_flush_data);
}*/
