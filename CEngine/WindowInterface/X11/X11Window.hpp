/**********************************************************************
File name: X11Window.hpp
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
#ifndef _PYUNI_X11_WINDOW_H
#define _PYUNI_X11_WINDOW_H

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

#include "WindowInterface/Window.hpp"
#include "IO/Log.hpp"

namespace PyUni {

/**
 * Create and manipulate an X11 window.
 */
class X11Window : public Window {
public:
    X11Window(::Display *display, XVisualInfo *vinfo,
              GLXFBConfig config, GLXContext context, int w, int h);
    virtual ~X11Window();

    virtual void flip();
    virtual void switchTo();

    /**
     * Set the window title.
     */
    virtual void setTitle(const char *title);

    /**
     * \rst
        Puts the window in fullscreen mode, using the EWMH standard which
        can be found at: 
        <http://standards.freedesktop.org/wm-spec/wm-spec-latest.html>.
        The implementation was written against 1.5-draft1.

        The parameters *top*, *bottom*, *left* and *right* must be the
        indicies of the monitors which represent the respective edges.

        To span from monitor A to monitor B horizontically, one would thus
        pass A, A, A, B, where the first two A's could be replaced with B's
        too.

        This is known to work with xfwm4. Other window managers will do, if
        they follow the spec. A fallback to raw X11 attacks should be done
        later.    
       \endrst
     */
    virtual void setFullscreen(int top, int bottom, int left, int right);

    /**
     * Puts the window in windowed mode, setting the width and height
     * accordingly.
     */
    virtual void setWindowed(int screen, int w, int h);

private:
    /**
     * Setup the window with the given width and height.
     */
    ::Window setupWindow(int w, int h);
    void setTextProperty(const char *atom, const char *value, bool utf8=true);

    ::Display *_display;
    XVisualInfo *_x_visual;
    GLXContext _glx_context;
    ::Window _win;
    GLXWindow _glx_win;
    // Colormap _x_colormap;

    LogChannelHandle _log;
};

}

#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
