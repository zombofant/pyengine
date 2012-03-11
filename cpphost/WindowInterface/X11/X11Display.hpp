
#ifndef PYUNI_X11_DISPLAY_H

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <X11/extensions/Xinerama.h>

#include "../Display.hpp"
#include "../EventSink.h"

namespace PyUni {
class X11Display : public Display {
public:
    X11Display(const char *foo = 0);
    virtual ~X11Display();

    virtual Window *createWindow(int w, int h, bool fullscreen=false);
    virtual void selectMode(int index);

    virtual void handleEvents(EventSink *eventSink);
private:
    void detectScreens();
    void detectDisplayModes();

    int _config;
    GLXFBConfig *_configs;
    GLXContext _glx_context;
    XVisualInfo *_x_visual;
    ::Display *_display;
};
}

#endif

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
