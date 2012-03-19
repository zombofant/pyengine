/**********************************************************************
File name: X11Display.hpp
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

#ifndef _PYUNI_X11_DISPLAY_H
#define _PYUNI_X11_DISPLAY_H

#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>

#include "../Display.hpp"
#include "../EventSink.hpp"

namespace PyUni {
class X11Display : public Display {
public:
    X11Display(const char *foo = 0);
    virtual ~X11Display();

    virtual WindowHandle createWindow(const DisplayMode &mode,
        int w, int h, bool fullscreen=false);

    virtual void pullEvents(EventSink *sink);
private:
    void detectScreens();
    void detectDisplayModes();
    void openInputContext();

    ::Display *_display;
    Atom _wm_quit, _wm_protocols;
    XIC _input_context;
    int _mouse_x, _mouse_y;
    int *_scroll_x,
        *_scroll_y;
    bool _mouse_valid;
};

}

#endif

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
