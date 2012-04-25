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
#include "X11Display.hpp"

#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "WindowInterface/Display.hpp"
#include "X11Window.hpp"

namespace PyUni {

X11Display::X11Display(const char *display):
    Display::Display()
{
    if (display == NULL) {
        display = getenv("DISPLAY");
    }

    if (display == NULL) {
        display = ":0";
    }

    _display = XOpenDisplay(display);

    _mouse_x = 0;
    _mouse_y = 0;
    _mouse_valid = false;

    _wm_quit = XInternAtom(_display, "WM_DELETE_WINDOW", False);
    _wm_protocols = XInternAtom(_display, "WM_PROTOCOLS", False);

    this->detectScreens();
    this->detectDisplayModes();

    // this should go somewhere else
    // but for now it has to stay here
    this->openInputContext();
}

X11Display::~X11Display() {
    XDestroyIC(_input_context);
    XCloseDisplay(_display);
}

WindowHandle X11Display::createWindow(const DisplayMode &mode, int w, int h, bool fullscreen) {
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
        GLX_SAMPLES         , mode.samples,
        None
    };

    int count;
    GLXFBConfig *configs = glXChooseFBConfig(_display, DefaultScreen(_display), reqAttribs, &count);
    if (count == 0) {
        // TODO: Raise an error here
        std::cerr << "No config found for: " << mode << std::endl;
        return WindowHandle();
    }

    glxContext = glXCreateNewContext(_display, configs[0], GLX_RGBA_TYPE, NULL, True);
    xVisual = glXGetVisualFromFBConfig(_display, configs[0]);


    X11Window *win = new X11Window(_display, xVisual, configs[0], glxContext, w, h);
    XFree(configs);
    return WindowHandle(win);
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
        // should we assert that, if not: how do we define
        // the positions of the screens, stretching windows
        // will be impossible then anyway
        // and it seems we want one large window ...
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

void X11Display::openInputContext() {
    // default input method for now
    XIM im = XOpenIM(_display, NULL, NULL, NULL);
    _input_context = XCreateIC(im,
                               XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                               NULL);
    XSetICFocus(_input_context);
}

void X11Display::pullEvents(EventSink *sink) {

    while (XPending(_display)) {
        XEvent event;
        KeySym keysym;

        XNextEvent(_display, &event);

        if (XFilterEvent(&event, None))
            continue;

        switch (event.type) {
        case ButtonPress:
            sink->handleMouseDown(event.xbutton.x,
                                  event.xbutton.y,
                                  event.xbutton.button,
                                  event.xbutton.state);
            break;
        case ButtonRelease:
            sink->handleMouseUp(event.xbutton.x,
                                event.xbutton.y,
                                event.xbutton.button,
                                event.xbutton.state);
            break;
        case MotionNotify:
            sink->handleMouseMove(event.xmotion.x,
                                  event.xmotion.y,
                                  _mouse_valid ? event.xmotion.x - _mouse_x : 0,
                                  _mouse_valid ? event.xmotion.y - _mouse_y : 0,
                                  event.xmotion.state,
                                  event.xmotion.state);

            _mouse_x = event.xmotion.x;
            _mouse_y = event.xmotion.y;
            _mouse_valid = true;
            break;
        case KeyPress: {
            // XXX: this will break with threading
            // avoid repeated reallocation of the text buffer
            static int buf_size = 0;
            static char *buf = NULL;

            Status ret_state;

            int len = Xutf8LookupString(_input_context,
                                        &event.xkey,
                                        buf,
                                        buf_size,
                                        &keysym,
                                        &ret_state);

            if (ret_state == XBufferOverflow) {
                buf_size = len + 1;
                buf = (char *) realloc(buf, buf_size);

                if (buf == NULL) {
                    // FIXME: raise exception, die ungracefully but loudly
                }

                len = Xutf8LookupString(_input_context,
                                        &event.xkey,
                                        buf,
                                        len,
                                        &keysym,
                                        &ret_state);

            }

            // fprintf(stderr, "Text Input: ");
            // fwrite(buf, 1, len, stderr);
            // fprintf(stderr, "\n");

            if (ret_state == XLookupBoth || ret_state == XLookupKeySym) {
                sink->handleKeyDown(keysym, event.xkey.state);
            }

            if (ret_state == XLookupBoth || ret_state == XLookupChars) {
                // fully filled buffer, we need one more byte to zero terminate
                if (len == buf_size) {
                    // make space for
                    buf = (char *) realloc(buf, ++buf_size);
                    if (buf == NULL) {
                        // FIXME: raise exception, die ungracefully but loudly
                    }
                }

                // add zero termination to make it a C string
                buf[len] = '\0';

                sink->handleTextInput(buf);
            }
        } break;
        case KeyRelease:
            sink->handleKeyUp(XLookupKeysym(&event.xkey, /* ??? */ 0), event.xkey.state);
            break;
        case ConfigureNotify:
            sink->handleResize(event.xconfigure.width, event.xconfigure.height);
            break;
        case ClientMessage:
            if (event.xclient.message_type == _wm_protocols) {
                if ((Atom)event.xclient.data.l[0] == _wm_quit) {
                    sink->handleWMQuit();
                } else {
                    char *name = XGetAtomName(_display, event.xclient.data.l[0]);
                    fprintf(stderr, "Unknown X WM Protocol message %s!\n", name);
                    XFree(name);
                }
            } else {
                char *name = XGetAtomName(_display, event.xclient.message_type);
                fprintf(stderr, "Unknown X ClientMessage type %s!\n", name);
                XFree(name);
            }
            break;
        default:
            fprintf(stderr, "Unknown X Event (type %d)!\n", event.type);
            break;
        }
    }
}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
