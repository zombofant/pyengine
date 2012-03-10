
#ifndef PYUNI_X11_DISPLAY_H

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <X11/extensions/Xinerama.h>

#include "Display.hpp"

namespace PyUni {
class X11Display : public Display {
public:
    X11Display(const char *foo = 0);
    virtual ~X11Display();
private:
    void detectScreens();
    void detectDisplayModes();

    GLXFBConfig *_configs;
    ::Display *_display;
};
}

#endif

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
