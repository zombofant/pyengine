/**********************************************************************
File name: X11Display.cpp
This file is part of: Pythonic Engine

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

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "X11Display.hpp"

#include <cstdlib>
#include <cstdio>
#include <iostream>

#define MAXBUTTONS 16

#include "CEngine/WindowInterface/Display.hpp"
#include "CEngine/Misc/Int.hpp"

#include "X11Window.hpp"

namespace PyEngine {

X11Display::X11Display(const char *display):
    Display::Display(),
    _log(log->getChannel("x11"))
{
    if (display == NULL) {
        display = getenv("DISPLAY");
    }

    if (display == NULL) {
        display = ":0";
    }

    _display = XOpenDisplay(display);
    if (!_display) {
        _log->logf(Error, "X11 Display connection failed to display \"%s\".", display);
        throw DisplayError("Could not connect to X11 display.");
    }
    _log->logf(Information, "Connected to X11 display \"%s\".", display);

    _mouse_x = 0;
    _mouse_y = 0;
    _mouse_valid = false;

    _log->logf(Debug, "Caching atoms");
    _wm_quit = XInternAtom(_display, "WM_DELETE_WINDOW", False);
    _wm_protocols = XInternAtom(_display, "WM_PROTOCOLS", False);

    this->detectScreens();
    this->detectDisplayModes();

    // this should go somewhere else
    // but for now it has to stay here
    // AND: this might need config for someone else
    this->openInputContext();

    // FIXME: This should be configurable (or at least discoverable)
    // ask the pyglet source ... okay they just have scrolly
    // and it is hardcoded
    _scroll_x = (int *) malloc(sizeof(int) * MAXBUTTONS);
    _scroll_y = (int *) malloc(sizeof(int) * MAXBUTTONS);

    for (int i = 0; i < MAXBUTTONS; i++) {
        _scroll_x[i] = 0;
        _scroll_y[i] = 0;
    }
    _scroll_y[4] = 1;
    _scroll_y[5] = -1;
    _scroll_x[6] = 1;
    _scroll_x[7] = -1;

    for (unsigned int i = 0; i < MAX_MULTICLICK_BUTTON; i++) {
        _last_mouse_down[i] = nanotime();
        _nclick[i] = 1;
        _nclick_pos[i][0] = -10;
        _nclick_pos[i][1] = -10;
    }

    _log->logf(Information, "X11 interface set up successfully.");
}

X11Display::~X11Display() {
    XDestroyIC(_input_context);
    XCloseDisplay(_display);
    free(_scroll_x);
    free(_scroll_y);
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
        _log->log(Error) << "No config found for mode: " << mode << submit;
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
        _log->logf(Information, "Xinerama available");
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
        _log->logf(Warning, "Xinerama not supported. No proper multi-screen support will be available");
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
    _log->logf(Information, "Detected %d screens", _screens.size());
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
        None
    };
    _log->logf(Debug, "Enumerating display modes");
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
        if (!hasDisplayMode(instance)) {
            _displayModes.push_back(instance);
            _log->log(Debug) << "Display mode: " << instance << submit;
        }
    }
    _log->logf(Information, "Found %d display modes", _displayModes.size());
    XFree(configs);
}

void X11Display::openInputContext() {
    _log->logf(Information, "Opening input context");
    // default input method for now
    XIM im = XOpenIM(_display, NULL, NULL, NULL);
    _input_context = XCreateIC(im,
                               XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                               NULL);
    if (_input_context == NULL) {
        _log->logf(Error, "Could not acquire input context");
        // FIXME: die loudly, or try to correct
    }

    XSetICFocus(_input_context);
}

void X11Display::pullEvents(EventSink *sink) {

    while (XPending(_display)) {
        XEvent event;
        KeySym keysym;

        XNextEvent(_display, &event);

        // interaction with the input method
        if (XFilterEvent(&event, None))
            continue;

        switch (event.type) {
        case ButtonPress:
            assert(event.xbutton.button < MAXBUTTONS);

            // filter scroll events
            if (_scroll_x[event.xbutton.button] != 0
             || _scroll_y[event.xbutton.button] != 0) {
                sink->dispatch_scroll(
                    event.xbutton.x,
                    event.xbutton.y,
                    _scroll_x[event.xbutton.button],
                    _scroll_y[event.xbutton.button]);
            } else {
                sink->dispatch_mouse_down(
                    event.xbutton.x,
                    event.xbutton.y,
                    event.xbutton.button,
                    event.xbutton.state);
            }
            break;
        case ButtonRelease:
            assert(event.xbutton.button < MAXBUTTONS);

            if (event.xbutton.button <= MAX_MULTICLICK_BUTTON) {
                assert(event.xbutton.button != 0);
                TimeStamp &previous = _last_mouse_down[event.xbutton.button-1];
                const TimeStamp now = nanotime();
                TimeFloat interval = timeIntervalToDouble(
                    previous,
                    now
                    );

                previous = now;

                unsigned int &nclick = _nclick[event.xbutton.button-1];
                int (&nclick_pos)[2] = _nclick_pos[event.xbutton.button-1];

                const int dist = nclick_pos[0] + nclick_pos[1] -
                    event.xbutton.x - event.xbutton.y;

                if (interval <= NCLICK_TIME &&
                    (dist <= MAX_NCLICK_DISTANCE))
                {
                    if (nclick < MAX_NCLICK)
                        nclick += 1;
                    else
                        nclick = 1;
                } else {
                    nclick = 1;
                }
                nclick_pos[0] = event.xbutton.x;
                nclick_pos[1] = event.xbutton.y;

                _log->logf(Debug, "nclick %d with button %d",
                           nclick, event.xbutton.button);

                sink->dispatch_mouse_click(
                    event.xbutton.x,
                    event.xbutton.y,
                    event.xbutton.button,
                    event.xbutton.state,
                    nclick);
            }

            // only report release events for non-scroll
            if (_scroll_x[event.xbutton.button] == 0
             && _scroll_y[event.xbutton.button] == 0) {
                sink->dispatch_mouse_up(
                    event.xbutton.x,
                    event.xbutton.y,
                    event.xbutton.button,
                    event.xbutton.state);
            }
            break;
        case MotionNotify:
            sink->dispatch_mouse_move(
                event.xmotion.x,
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
            // avoids repeated reallocation of the text buffer
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
                    _log->log(Error, "Memory allocation for utf8 buffer failed.");
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
                sink->dispatch_key_down((Key::Key)(keysym),
                                        event.xkey.state);
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

                sink->dispatch_text_input(buf);
            }
        } break;
        case KeyRelease:
            sink->dispatch_key_up(
                (Key::Key)XLookupKeysym(&event.xkey, /* ??? */ 0),
                event.xkey.state);
            break;
        case ConfigureNotify:
            sink->dispatch_resize(event.xconfigure.width, event.xconfigure.height);
            break;
        case MapNotify:
            sink->dispatch_show();
            break;
        case UnmapNotify:
            sink->dispatch_hide();
            break;
        case ReparentNotify:
            // XXX: debug log this, handle this if we are interested in it some day
            break;
        case ClientMessage:
            if (event.xclient.message_type == _wm_protocols) {
                if ((Atom)event.xclient.data.l[0] == _wm_quit) {
                    sink->dispatch_wm_quit();
                } else {
                    char *name = XGetAtomName(_display, event.xclient.data.l[0]);
                    _log->logf(Warning, "Unknown X WM Protocol message %s!\n", name);
                    XFree(name);
                }
            } else {
                char *name = XGetAtomName(_display, event.xclient.message_type);
                _log->logf(Warning, "Unknown X ClientMessage type %s!\n", name);
                XFree(name);
            }
            break;
        default:
            _log->logf(Warning, "Unknown X Event (type %d)!", event.type);
            break;
        }
    }
}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
