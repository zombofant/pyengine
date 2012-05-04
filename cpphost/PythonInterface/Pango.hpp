/**********************************************************************
File name: Pango.hpp
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
        double getResolution();
        void layoutPath(PangoLayoutHandle layout);
        void setResolution(double dpi);
        void showLayout(PangoLayoutHandle layout);
        void updateContext();
};

typedef boost::shared_ptr<PangoCairoContext> PangoCairoContextHandle;

class PangoLayout {
    public:
        PangoLayout(PangoCairoContextHandle ctx);
        virtual ~PangoLayout();
    private:
        ::PangoLayout *_layout;
    public:
        void contextChanged();

        PangoAlignment getAlignment();
        PangoEllipsizeMode getEllipsize();
        int getHeight();
        bool getJustify();
        ::PangoLayout *getPangoLayout();
        const char *getText();
        int getWidth();
        PangoWrapMode getWrap();

        bool isWrapped();
        bool isEllipsized();

        void setAlignment(PangoAlignment alignment);
        void setEllipsize(PangoEllipsizeMode ellipsize);
        void setHeight(int height);
        void setJustify(bool justify);
        void setMarkup(const char *markup, int length);
        void setText(const char *text, int length);
        void setWidth(int width);
        void setWrap(PangoWrapMode mode);
};

void addPangoToInittab();

}

#endif
