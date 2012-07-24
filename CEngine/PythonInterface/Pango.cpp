/**********************************************************************
File name: Pango.cpp
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
#include "Pango.hpp"

#include <boost/python.hpp>
#include <glib.h>
#include <pango/pangocairo.h>
#include <pycairo/pycairo.h>

#include "Helpers.hpp"
#include "CairoHelpers.hpp"
#include "Helpers.hpp"

namespace PyUni {

/* PyUni::PangoCairoContext */

PangoCairoContext::PangoCairoContext(cairo_t *cairoCtx):
    _cairoCtx(cairoCtx),
    _ctx(pango_cairo_create_context(cairoCtx))
{

}

PangoCairoContext::~PangoCairoContext()
{
    g_object_unref(_ctx);
}

PangoContext *PangoCairoContext::getPangoContext()
{
    return _ctx;
}

double PangoCairoContext::getResolution()
{
    return pango_cairo_context_get_resolution(_ctx);
}

void PangoCairoContext::layoutPath(PangoLayoutHandle layout)
{
    pango_cairo_layout_path(_cairoCtx, layout->getPangoLayout());
}

void PangoCairoContext::setResolution(double dpi)
{
    pango_cairo_context_set_resolution(_ctx, dpi);
}

void PangoCairoContext::showLayout(PangoLayoutHandle layout)
{
    pango_cairo_show_layout(_cairoCtx, layout->getPangoLayout());
}

void PangoCairoContext::updateContext()
{
    pango_cairo_update_context(_cairoCtx, _ctx);
}

/* PyUni::PangoLayout */

PangoLayout::PangoLayout(PangoCairoContextHandle ctx):
    _layout(pango_layout_new(ctx->getPangoContext()))
{
    PangoFontDescription *descr = pango_font_description_new();
    pango_font_description_set_family(descr, "Cantarell");
    pango_font_description_set_size(descr, 12*PANGO_SCALE);
    pango_layout_set_font_description(_layout, descr);
    pango_font_description_free(descr);
}

PangoLayout::~PangoLayout()
{
    g_object_unref(_layout);
}

void PangoLayout::contextChanged()
{
    pango_layout_context_changed(_layout);
}

PangoAlignment PangoLayout::getAlignment()
{
    return pango_layout_get_alignment(_layout);
}

PangoEllipsizeMode PangoLayout::getEllipsize()
{
    return pango_layout_get_ellipsize(_layout);
}

int PangoLayout::getHeight()
{
    return pango_layout_get_height(_layout);
}

bool PangoLayout::getJustify()
{
    return (pango_layout_get_justify(_layout) == FALSE ? false : true);
}

::PangoLayout *PangoLayout::getPangoLayout()
{
    return _layout;
}

const char *PangoLayout::getText()
{
    return pango_layout_get_text(_layout);
}

int PangoLayout::getWidth()
{
    return pango_layout_get_width(_layout);
}

PangoWrapMode PangoLayout::getWrap()
{
    return pango_layout_get_wrap(_layout);
}

bool PangoLayout::isWrapped()
{
    return pango_layout_is_wrapped(_layout);
}

bool PangoLayout::isEllipsized()
{
    return pango_layout_is_ellipsized(_layout);
}

void PangoLayout::setAlignment(PangoAlignment alignment)
{
    pango_layout_set_alignment(_layout, alignment);
}

void PangoLayout::setEllipsize(PangoEllipsizeMode ellipsize)
{
    pango_layout_set_ellipsize(_layout, ellipsize);
}

void PangoLayout::setHeight(int height)
{
    pango_layout_set_height(_layout, height);
}

void PangoLayout::setJustify(bool justify)
{
    pango_layout_set_justify(_layout, (justify ? TRUE : FALSE));
}

void PangoLayout::setMarkup(const char *markup, int length)
{
    pango_layout_set_markup(_layout, markup, length);
}

void PangoLayout::setText(const char *text, int length)
{
    pango_layout_set_text(_layout, text, length);
}

void PangoLayout::setWidth(int width)
{
    pango_layout_set_width(_layout, width);
}

void PangoLayout::setWrap(PangoWrapMode mode)
{
    pango_layout_set_wrap(_layout, mode);
}

using namespace boost::python;

PangoCairoContextHandle bp_createPangoContext(PyObject *contextObj)
{
    if (!PyObject_IsInstance(contextObj, (PyObject *)Pycairo_CAPI->Context_Type)) {
        PyErr_SetString(PyExc_TypeError, "Context required");
        throw error_already_set();
    }
    cairo_t *context = ((PycairoContext*)contextObj)->ctx;
    return PangoCairoContextHandle(new PangoCairoContext(context));
}

PyObject *bp_PangoLayout_getText(PangoLayoutHandle layout)
{
    const char *text = layout->getText();
    Py_ssize_t len = strlen(text);
    return PyUnicode_DecodeUTF8(text, len, 0);
}

void bp_PangoLayout_setText(PangoLayoutHandle layout, PyObject *rawText)
{
    const char *data = 0;
    int len = 0;
    PyObject *text = extractUTF8String(rawText, &data, &len);
    // FIXME: length check here.
    layout->setText(data, len);

    Py_DECREF(text);
}

void bp_PangoLayout_setMarkup(PangoLayoutHandle layout, PyObject *rawText)
{
    const char *data = 0;
    int len = 0;
    PyObject *text = extractUTF8String(rawText, &data, &len);
    // FIXME: length check here.
    layout->setMarkup(data, len);

    Py_DECREF(text);
}


BOOST_PYTHON_MODULE(_cuni_pango)
{
    class_<PangoCairoContext, PangoCairoContextHandle>("PangoCairoContext", no_init)
        .def("__init__", make_constructor(&bp_createPangoContext))
        .def("layoutPath", &PangoCairoContext::layoutPath)
        .def("showLayout", &PangoCairoContext::showLayout)
        .def("updateContext", &PangoCairoContext::updateContext)
        .add_property("Resolution", &PangoCairoContext::getResolution, &PangoCairoContext::setResolution)
    ;

    class_<PangoLayout, PangoLayoutHandle>("PangoLayout", init<PangoCairoContextHandle>())
        .def("contextChanged", &PangoLayout::contextChanged)
        .def("setMarkup", &bp_PangoLayout_setMarkup)
        .add_property("Alignment", &PangoLayout::getAlignment, &PangoLayout::setAlignment)
        .add_property("Ellipsize", &PangoLayout::getEllipsize, &PangoLayout::setEllipsize)
        .add_property("Height", &PangoLayout::getHeight, &PangoLayout::setHeight)
        .add_property("Justify", &PangoLayout::getJustify, &PangoLayout::setJustify)
        .add_property("Text", bp_PangoLayout_getText, bp_PangoLayout_setText)
        .add_property("Width", &PangoLayout::getWidth, &PangoLayout::setWidth)
        .add_property("Wrap", &PangoLayout::getWrap, &PangoLayout::setWrap)
        .add_property("IsEllipsized", &PangoLayout::isEllipsized)
        .add_property("IsWrapped", &PangoLayout::isWrapped)
    ;

    object module = scope();
    module.attr("Scale") = PANGO_SCALE;

    enum_<PangoEllipsizeMode>("EllipsizeMode")
        .value("None", PANGO_ELLIPSIZE_NONE)
        .value("Start", PANGO_ELLIPSIZE_START)
        .value("Middle", PANGO_ELLIPSIZE_MIDDLE)
        .value("End", PANGO_ELLIPSIZE_END)
    ;

    enum_<PangoWrapMode>("WrapMode")
        .value("Char", PANGO_WRAP_CHAR)
        .value("Word", PANGO_WRAP_WORD)
        .value("WordChar", PANGO_WRAP_WORD_CHAR)
    ;

    enum_<PangoAlignment>("Alignment")
        .value("Center", PANGO_ALIGN_CENTER)
        .value("Left", PANGO_ALIGN_LEFT)
        .value("Right", PANGO_ALIGN_RIGHT)
    ;
}

void addPangoToInittab()
{
    PyImport_AppendInittab("_cuni_pango", &init_cuni_pango);
}

}
