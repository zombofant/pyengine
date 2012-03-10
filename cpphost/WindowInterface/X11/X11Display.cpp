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

#include <unistd.h>

#include "X11Display.h"
#include "../Display.h"

namespace PyUni {
X11Display::X11Display(const char *display) {
    if (display == 0) {
        display = getenv("DISPLAY");
    }

    if (display == 0) {
        display = ":0";
    }

    _display = XOpenDisplay(display);
}

X11Display::~X11Display() {
    XFree(_configs);
    XCloseDisplay(_display);
}

Window X11Display::createWindow() {
    return new X11Window(_display);
}

void X11Display::detectScreens() {
    int event_base_return, error_base_return;
    screens.clear();

    // randr, twinview and real xinerama all implement the Xinerama
    // interface for clients so that should catch it all
    if (XineramaQueryExtenison(_display,
                               &event_base_return,
                               &error_base_return)
        && XineramaIsActive(_display)) {
        int number;
        XineramaScreenInfo *screens = XineramaQueryScreens(_display,
                                                           &number);
        for (int i = 0; i < number; i++) {
            screens.push_back(Screen(screens[i].x_org,
                                     screens[i].y_org,
                                     screens[i].x_height,
                                     screens[i].width,
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
            screens.push_back(Screen(0,
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

    displayModes.clear();

    for (int i = 0; i < count; i++) {
        int redBits, greenBits, blueBits, alphaBits, depthBits,
            stencilBits, doubleBuffered;

        glXGetFBConfigAttrib(_display, _configs[i], GLX_RED_SIZE, &redBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_GREEN_SIZE, &greenBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_BLUE_SIZE, &blueBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_ALPHA_SIZE, &alphaBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_DEPTH_SIZE, &depthBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_STENCIL_SIZE, &stencillBits);
        glXGetFBConfigAttrib(_display, _configs[i], GLX_DOUBLEBUFFER, &doubleBuffered);

        displayModes.push_back(DisplayMode(redBits,
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
