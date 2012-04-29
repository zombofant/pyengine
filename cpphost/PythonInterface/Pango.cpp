#include "Pango.hpp"

#include <boost/python.hpp>
#include <glib.h>
#include <pango/pangocairo.h>
#include <pycairo/pycairo.h>

#include "CairoHelpers.hpp"

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

void PangoCairoContext::layoutPath(PangoLayoutHandle layout)
{
    pango_cairo_layout_path(_cairoCtx, layout->getPangoLayout());
}

void PangoCairoContext::showLayout(PangoLayoutHandle layout)
{
    pango_cairo_show_layout(_cairoCtx, layout->getPangoLayout());
}

/* PyUni::PangoLayout */

PangoLayout::PangoLayout(PangoCairoContextHandle ctx):
    _layout(pango_layout_new(ctx->getPangoContext()))
{

}

PangoLayout::~PangoLayout()
{
    g_object_unref(_layout);
}

PangoEllipsizeMode PangoLayout::getEllipsize()
{
    return pango_layout_get_ellipsize(_layout);
}

int PangoLayout::getHeight()
{
    return pango_layout_get_height(_layout);
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

void PangoLayout::setEllipsize(PangoEllipsizeMode ellipsize)
{
    return pango_layout_set_ellipsize(_layout, ellipsize);
}

void PangoLayout::setHeight(int height)
{
    return pango_layout_set_height(_layout, height);
}

void PangoLayout::setText(const char *text, int length)
{
    return pango_layout_set_text(_layout, text, length);
}

void PangoLayout::setWidth(int width)
{
    return pango_layout_set_width(_layout, width);
}

void PangoLayout::setWrap(PangoWrapMode mode)
{
    return pango_layout_set_wrap(_layout, mode);
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

BOOST_PYTHON_MODULE(_cuni_pango)
{
    class_<PangoCairoContext, PangoCairoContextHandle>("PangoCairoContext", no_init)
        .def("__init__", make_constructor(&bp_createPangoContext))
        .def("layoutPath", &PangoCairoContext::layoutPath)
        .def("showLayout", &PangoCairoContext::showLayout)
    ;

    class_<PangoLayout, PangoLayoutHandle>("PangoLayout", init<PangoCairoContextHandle>())
        .add_property("Ellipsize", &PangoLayout::getEllipsize, &PangoLayout::setEllipsize)
        .add_property("Height", &PangoLayout::getHeight, &PangoLayout::setHeight)
        .add_property("Text", &PangoLayout::getText, &PangoLayout::setText)
        .add_property("Width", &PangoLayout::getWidth, &PangoLayout::setWidth)
        .add_property("Wrap", &PangoLayout::getWrap, &PangoLayout::setWrap)
        .add_property("IsEllipsized", &PangoLayout::isEllipsized)
        .add_property("IsWrapped", &PangoLayout::isWrapped)
    ;
}

void addPangoToInitTab()
{
    PyImport_AppendInittab("_cuni_pango", &init_cuni_pango);
}

}
