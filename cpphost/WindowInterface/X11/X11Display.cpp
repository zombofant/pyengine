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

#include <iostream>

namespace PyUni {
X11Display::X11Display(const char *display) {
    if (display == NULL) {
        display = getenv("DISPLAY");
    }

    if (display == NULL) {
        display = ":0";
    }

    _display = XOpenDisplay(display);

    this->detectScreens();
    this->detectDisplayModes();
}

X11Display::~X11Display() {
    XCloseDisplay(_display);
}
/*    GLXContext _glx_context;
    XVisualInfo *_x_visual;
*/
Window *X11Display::createWindow(const DisplayMode &mode, int w, int h, bool fullscreen) {
    XVisualInfo *xVisual;
    GLXContext glxContext;
    
    static int reqAttribs[] = {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_DEPTH_SIZE      , mode.depthBits,
        GLX_RED_SIZE        , mode.redBits,
        GLX_GREEN_SIZE      , mode.greenBits,
        GLX_BLUE_SIZE       , mode.blueBits,
        GLX_ALPHA_SIZE      , mode.alphaBits,
        GLX_STENCIL_SIZE    , mode.stencilBits,
        GLX_DOUBLEBUFFER    , mode.doubleBuffered,
        None
    };

    int count;
    GLXFBConfig *configs = glXChooseFBConfig(_display, DefaultScreen(_display), reqAttribs, &count);
    if (count == 0) {
        // TODO: Raise an error here
        std::cerr << "No config found for: " << mode << std::endl;
        return 0;
    }
    
    glxContext = glXCreateNewContext(_display, configs[0], GLX_RGBA_TYPE, NULL, True);
    xVisual = glXGetVisualFromFBConfig(_display, configs[0]);
    

    X11Window *win = new X11Window(_display, xVisual, configs[0], glxContext, w, h);
    XFree(configs);
    return win;
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

    // we ignore all configs not matching at least these required attribs:
    static int reqAttribs[] = {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_DEPTH_SIZE      , 16,
        None
    };
    GLXFBConfig *configs = glXChooseFBConfig(_display, DefaultScreen(_display), reqAttribs, &count);

    /*// select best mode (mode with highest sample rate)
    int bestMode = -1, samplesMax = -1;
    for(int i = 0; i < count; ++i) {
        XVisualInfo *vi = glXGetVisualFromFBConfig(_display, _configs[i]);
        if(vi) {
            int samples, sampleBuffers;
            glXGetFBConfigAttrib(_display, _configs[i], GLX_SAMPLES, &samples);
            glXGetFBConfigAttrib(_display, _configs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
            if(bestMode < 0 || sampleBuffers && samples > samplesMax) {
                bestMode = i;
                samplesMax = samples;
            }
        }
        XFree(vi);
    }
    // best mode's index is not in bestMode */

    _displayModes.clear();

    for (int i = 0; i < count; i++) {
        int redBits, greenBits, blueBits, alphaBits, depthBits,
            stencilBits, doubleBuffered, samples;

        glXGetFBConfigAttrib(_display, configs[i], GLX_RED_SIZE, &redBits);
        glXGetFBConfigAttrib(_display, configs[i], GLX_GREEN_SIZE, &greenBits);
        glXGetFBConfigAttrib(_display, configs[i], GLX_BLUE_SIZE, &blueBits);
        glXGetFBConfigAttrib(_display, configs[i], GLX_ALPHA_SIZE, &alphaBits);
        glXGetFBConfigAttrib(_display, configs[i], GLX_DEPTH_SIZE, &depthBits);
        glXGetFBConfigAttrib(_display, configs[i], GLX_STENCIL_SIZE, &stencilBits);
        glXGetFBConfigAttrib(_display, configs[i], GLX_DOUBLEBUFFER, &doubleBuffered);
        glXGetFBConfigAttrib(_display, configs[i], GLX_SAMPLES, &samples);

        const DisplayMode instance = DisplayMode(redBits,
                                            greenBits,
                                            blueBits,
                                            alphaBits,
                                            depthBits,
                                            stencilBits,
                                            samples,
                                            doubleBuffered);
        if (!hasDisplayMode(instance))
            _displayModes.push_back(instance);
    }
    XFree(configs);
}
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
