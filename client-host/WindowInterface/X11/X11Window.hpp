
#ifndef PYUNI_X11_WINDOW_H
#define PYUNI_X11_WINDOW_H

#include "../Window.hpp"

namespace PyUni {
class X11Window : public Window {
public:
    X11Window();
    virtual ~X11Window();

    virtual flip();
    virtual switchTo();
private:
    ::Window setupWindow(int w, int h);

    ::Window _win;
    GLXWindow _glx_win;
    GCXContext _glx_context;
    XVisualInfo *_x_visual;
    // Colormap _x_colormap;
};

}

#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
