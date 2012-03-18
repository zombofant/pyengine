/**********************************************************************
File name: X11Window.cpp
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

#include "X11Window.hpp"

namespace PyUni {

X11Window::X11Window(::Display *disp,
                     XVisualInfo *vinfo,
                     GLXFBConfig config,
                     GLXContext context,
                     int w,
                     int h) {
    _display = disp;
    _x_visual = vinfo;
    _glx_context = context;

    _win = setupWindow(w, h);
    _glx_win = glXCreateWindow(_display, config, _win, NULL);
}

X11Window::~X11Window() {
    glXMakeCurrent(_display, 0, 0);
    glXDestroyWindow(_display, _glx_win);
    XDestroyWindow(_display, _win);
}

::Window X11Window::setupWindow(int w, int h) {
    XSetWindowAttributes attrs;
    int attr_mask = CWBorderPixel | CWColormap | CWEventMask;

    attrs.border_pixel = 0;
    attrs.event_mask = StructureNotifyMask;
    attrs.colormap = XCreateColormap(_display,
                                     RootWindow(_display, _x_visual->screen),
                                     _x_visual->visual,
                                     AllocNone);


    ::Window win = XCreateWindow(_display,
                                 RootWindow(_display, _x_visual->screen),
                                 0, 0, w, h,
                                 _x_visual->depth,
                                 CopyFromParent,
                                 InputOutput,
                                 _x_visual->visual,
                                 attr_mask, &attrs);

    // setup window manager interaction
    Atom protos[1];
    protos[0] = XInternAtom(_display, "WM_DELETE_WINDOW", 1);
    XSetWMProtocols(_display, win, protos, 1);

    // TODO: set title

    // select input and map the window
    XSelectInput(_display, win,
                 ButtonPressMask
                 |ButtonReleaseMask
                 |StructureNotifyMask
                 |KeyPressMask);

    // glXSelectInput?

    XMapWindow(_display, win);

    glXWaitGL();
    glXWaitX();
    return win;
}

void X11Window::switchTo() {
    glXMakeCurrent(_display, _glx_win, _glx_context);
    glXMakeContextCurrent(_display, _glx_win, _glx_win, _glx_context);
}

void X11Window::flip() {
    // vsync goes here?
    // well better if all windows are flipped in parallel?
    // using one vsync wait in the main loop?

    glXSwapBuffers(_display, _glx_win);
}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
