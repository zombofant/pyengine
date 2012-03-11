
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

    return win;
}

void X11Window::switchTo() {
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
