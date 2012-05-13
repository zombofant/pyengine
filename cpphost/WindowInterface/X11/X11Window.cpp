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
#include <string.h>

#include <X11/Xatom.h>

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

    setTitle("Untitled");

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

    // select input and map the window
    XSelectInput(_display, win,
                 ButtonPressMask
                 |ButtonReleaseMask
                 |PointerMotionMask
                 |StructureNotifyMask
                 |KeyPressMask
                 |KeyReleaseMask);

    // glXSelectInput?

    XMapWindow(_display, win);

    glXWaitGL();
    glXWaitX();
    return win;
}

void X11Window::setTextProperty(const char *atom, const char *value, bool utf8) {
    XTextProperty p;
    Atom atom_ = XInternAtom(_display, atom, False);
    char *foo = (char *) value;
    if (utf8) {
        int status = Xutf8TextListToTextProperty(_display, &foo, 1,
                                                 XUTF8StringStyle, &p);
        if (status != 0) {
            printf("utf8 Property Status %d\n", status);
            // FIXME: fail loudly
        }
    } else {
        int status = XStringListToTextProperty(&foo, 1, &p);
        if (status == 0) {
            printf("Text Property Status %d\n", status);
            // FIXME: fail loudly
        }
    }
    XSetTextProperty(_display, _win, &p, atom_);
}

void X11Window::setTitle(const char *title) {
    // set title -- where do we get our title from?
    // how do we map the title back to ascii for WM_NAME/WM_ICON_NAME
    // well we don't ...
    this->setTextProperty("WM_NAME", title, false);
    this->setTextProperty("WM_ICON_NAME", title, false);
    this->setTextProperty("_NET_WM_NAME", title);
    this->setTextProperty("_NET_WM_ICON_NAME", title);
}

void X11Window::switchTo() {
    glXMakeCurrent(_display, _glx_win, _glx_context);
    glXMakeContextCurrent(_display, _glx_win, _glx_win, _glx_context);
}

void X11Window::setFullscreen(int top, int bottom, int left, int right) {
    // compare the freedesktop EWMH standard
    XEvent event;

    // just to be sure ...
    memset(&event, '\0', sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.window = _win;
    event.xclient.message_type =
        XInternAtom(_display, "_NET_WM_FULLSCREEN_MONITORS", False);

    event.xclient.format = 32;
    event.xclient.data.l[0] = top;    // top monitor
    event.xclient.data.l[1] = bottom; // bottom monitor
    event.xclient.data.l[2] = left;   // left monitor
    event.xclient.data.l[3] = right;  // right monitor
    event.xclient.data.l[4] = 1;      // application: normal

    XSendEvent(_display, RootWindow(_display, 0), False,
               SubstructureNotifyMask | SubstructureRedirectMask, &event);

    // just to be sure ...
    memset(&event, '\0', sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.window = _win;
    event.xclient.message_type =
        XInternAtom(_display, "_NET_WM_STATE", False);

    event.xclient.format = 32;
    // operation _NET_WM_STATE_ADD
    event.xclient.data.l[0] = 1;
    // first property
    event.xclient.data.l[1] =
        XInternAtom(_display, "_NET_WM_STATE_FULLSCREEN", False);
    // second property (0 means no second property)
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 1; // application type: normal
    event.xclient.data.l[4] = 0; // unused

    XSendEvent(_display, RootWindow(_display, 0), False,
               SubstructureNotifyMask | SubstructureRedirectMask, &event);
}

void X11Window::setWindowed(int top, int w, int h) {
    // FIXME: w and h are ignored currently
    // compare the freedesktop EWMH standard
    XEvent event;
    // just to be sure ...
    memset(&event, '\0', sizeof(event));
    event.xclient.type = ClientMessage;
    event.xclient.window = _win;
    event.xclient.message_type =
        XInternAtom(_display, "_NET_WM_STATE", False);

    event.xclient.format = 32;
    // operation _NET_WM_STATE_REMOVE
    event.xclient.data.l[0] = 0;
    // first property
    event.xclient.data.l[1] =
        XInternAtom(_display, "_NET_WM_STATE_FULLSCREEN", False);
    // second property (0 means no second property)
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 1; // application type: normal
    event.xclient.data.l[4] = 0; // unused

    XSendEvent(_display, RootWindow(_display, 0), False,
               SubstructureNotifyMask | SubstructureRedirectMask, &event);
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
