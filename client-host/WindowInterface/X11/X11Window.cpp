
#include "X11Window.hpp"

namespace PyUni {

X11Window::X11Window(::Display *disp) {
    _display = disp;
    _win = XCreateWindow();
    _glx_win = glXCreateWindow(_display, _win);
}

X11Window::~X11Window() {
}

void X11Window::setup() {
    glXMakeContextCurrent(_display, _glx_win, _glx_win, _glx_context);
}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
