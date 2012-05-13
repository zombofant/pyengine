/**********************************************************************
File name: X11Window.hpp
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
#ifndef _PYUNI_X11_WINDOW_H
#define _PYUNI_X11_WINDOW_H

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

#include "WindowInterface/Window.hpp"

namespace PyUni {
class X11Window : public Window {
public:
    X11Window(::Display *display, XVisualInfo *vinfo,
              GLXFBConfig config, GLXContext context, int w, int h);
    virtual ~X11Window();

    virtual void flip();
    virtual void switchTo();

    virtual void setTitle(const char *title);
    virtual void setFullscreen(int top, int bottom, int left, int right);
    virtual void setWindowed(int screen, int w, int h);

private:
    ::Window setupWindow(int w, int h);
    void setTextProperty(const char *atom, const char *value, bool utf8=true);

    ::Display *_display;
    ::Window _win;
    GLXWindow _glx_win;
    GLXContext _glx_context;
    XVisualInfo *_x_visual;
    // Colormap _x_colormap;
};

}

#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
