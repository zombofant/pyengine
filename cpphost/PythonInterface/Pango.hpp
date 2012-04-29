#ifndef _PYUNI_PYTHON_INTERFACE_PANGO
#define _PYUNI_PYTHON_INTERFACE_PANGO

#include <boost/shared_ptr.hpp>
#include <pango/pango.h>
#include <cairo/cairo.h>

namespace PyUni {

class PangoLayout;

typedef boost::shared_ptr<PangoLayout> PangoLayoutHandle;

class PangoCairoContext {
    public:
        PangoCairoContext(cairo_t *cairoCtx);
        virtual ~PangoCairoContext();
    private:
        cairo_t *_cairoCtx;
        PangoContext *_ctx;
    public:
        PangoContext *getPangoContext();
        void layoutPath(PangoLayoutHandle layout);
        void showLayout(PangoLayoutHandle layout);
};

typedef boost::shared_ptr<PangoCairoContext> PangoCairoContextHandle;

class PangoLayout {
    public:
        PangoLayout(PangoCairoContextHandle ctx);
        virtual ~PangoLayout();
    private:
        ::PangoLayout *_layout;
    public:
        PangoEllipsizeMode getEllipsize();
        int getHeight();
        ::PangoLayout *getPangoLayout();
        const char *getText();
        int getWidth();
        PangoWrapMode getWrap();

        bool isWrapped();
        bool isEllipsized();

        void setEllipsize(PangoEllipsizeMode ellipsize);
        void setHeight(int height);
        void setText(const char *text, int length);
        void setWidth(int width);
        void setWrap(PangoWrapMode mode);
};

void addPangoToInitTab();

}

#endif
