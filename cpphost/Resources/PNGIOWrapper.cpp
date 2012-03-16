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
