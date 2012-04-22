#ifndef _PYUNI_PYTHON_CAIRO_HELPERS_H
#define _PYUNI_PYTHON_CAIRO_HELPERS_H

#include <pycairo/pycairo.h>
#include <boost/python.hpp>

namespace PyUni {

void setupCairoHelpers();

extern Pycairo_CAPI_t *Pycairo_CAPI;

}

#endif
