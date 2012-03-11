/**********************************************************************
File name: X11Display.cpp
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

#include <stdlib.h>

#include "X11Display.hpp"
#include "X11Window.hpp"
#include "../Display.hpp"

namespace PyUni {
X11Display::X11Display(const char *display) {
    if (display == NULL) {
        display = getenv("DISPLAY");
    }

    if (display == NULL) {
        display = ":0";
    }

    _display = XOpenDisplay(display);

    _config = -1;
    _x_visual = NULL;
    _configs = NULL;

    this->detectScreens();
    this->detectDisplayModes();
}

X11Display::~X11Display() {
    XFree(_configs);
    XCloseDisplay(_display);
}

Window *X11Display::createWindow(int w, int h, bool fullscreen) {
    return new X11Window(_display, _x_visual, _configs[_config], _glx_context, w, h);
}

void X11Display::selectMode(int index) {
    // this should be done only once, and we should error check
    _glx_context = glXCreateNewContext(_display, _configs[index], GLX_RGBA_TYPE, NULL, True);
    _x_visual = glXGetVisualFromFBConfig(_display, _configs[index]);
    _config = index;
}

void X11Display::detectScreens() {
    int event_base_return, error_base_return;
    _screens.clear();

    // randr, twinview and real xinerama all implement the Xinerama
    // interface for clients so that should catch it all
    if (XineramaQueryExtension(_display,
                               &event_base_return,
                               &error_base_return)
        && XineramaIsActive(_display)) {
        int number;
        XineramaScreenInfo *screens = XineramaQueryScreens(_display,
                                                           &number);
        for (int i = 0; i < number; i++) {
            _screens.push_back(Screen(screens[i].x_org,
                                      screens[i].y_org,
                                      screens[i].width,
                                      screens[i].height,
                                      i,
                                      i == 0));
        }
        XFree(screens);
    } else {
        // anyone without xinerama should have only one screen
        // therefore x,y = 0,0
        int numOfScreens = ScreenCount(_display);
        int defaultScreen = DefaultScreen(_display);
        for (int i = 0; i < numOfScreens; i++) {
            ::Screen *s = ScreenOfDisplay(_display, i);
            _screens.push_back(Screen(0,
                                      0,
                                      WidthOfScreen(s),
                                      HeightOfScreen(s),
                                      i,
                                      i == defaultScreen));
        }
    }
}

void X11Display::detectDisplayModes() {
    int count;
    // perhaps we should query for the GLX extension first
    // but that is not that important right now, though it
    // may lead to strange failure (but who has X withou GLX?)
    _configs = glXGetFBConfigs(_display, DefaultScreen(_display), &count);

    _displayModes.clear();

    for (int i = 0; i < count; i++) {
        int redBits, greenBits, blueBits, alphaBits, depthBits,
            stencilBits, doubleBuffered;

        glXGetFBConfigAttrib(_display, _configs[i], GLX_RED_SIZE, &redBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_GREEN_SIZE, &greenBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_BLUE_SIZE, &blueBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_ALPHA_SIZE, &alphaBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_DEPTH_SIZE, &depthBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_STENCIL_SIZE, &stencilBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_DOUBLEBUFFER, &doubleBuffered);

        _displayModes.push_back(DisplayMode(redBits,
                                            greenBits,
                                            blueBits,
                                            alphaBits,
                                            depthBits,
                                            stencilBits,
                                            doubleBuffered,
                                            i));
    }
}
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
