#ifndef _PYUNI_RESOURCES_PNG_IO_WRAPPER_H
#define _PYUNI_RESOURCES_PNG_IO_WRAPPER_H

#include <png.h>
#include <istream>

using namespace std;

void png_init_io(png_structp data, istream &input);
void png_init_io(png_structp data, ostream &output);
void png_init_io(png_structp data, iostream &io);

#endif
