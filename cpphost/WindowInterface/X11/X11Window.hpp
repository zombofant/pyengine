
#ifndef PYUNI_X11_WINDOW_H
#define PYUNI_X11_WINDOW_H

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <X11/extensions/Xinerama.h>

#include "../Window.hpp"

namespace PyUni {
class X11Window : public Window {
public:
    X11Window(::Display *display, XVisualInfo *vinfo, GLXFBConfig config, GLXContext context, int w, int h);
    virtual ~X11Window();

    virtual void flip();
    virtual void switchTo();
private:
    ::Window setupWindow(int w, int h);

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
