/**********************************************************************
File name: Window.hpp
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
#ifndef _PYE_PYTHON_WINDOW_H
#define _PYE_PYTHON_WINDOW_H

#include <string>

#include <boost/python.hpp>

#include "CEngine/WindowInterface/Display.hpp"
#include "CEngine/WindowInterface/Window.hpp"
#include "CEngine/WindowInterface/EventSink.hpp"
#include "CEngine/WindowInterface/EventLoop.hpp"

namespace PyEngine {

class EventSinkWrap: public EventSink, public boost::python::wrapper<EventSink>
{
    virtual void frame_synced()
    {
        this->get_override("frame_synced")();
    }

    virtual void frame_unsynced(double deltaT)
    {
        this->get_override("frame_unsynced")(deltaT);
    }

    virtual void dispatch_key_down(unsigned int symbol, unsigned int modifiers)
    {
        this->get_override("dispatch_key_down")(symbol, modifiers);
    }

    virtual void dispatch_key_up(unsigned int symbol, unsigned int modifiers)
    {
        this->get_override("dispatch_key_up")(symbol, modifiers);
    }

    virtual void dispatch_mouse_down(int x, int y, unsigned int buttons, unsigned int modifiers)
    {
        this->get_override("dispatch_mouse_down")(x, y, buttons, modifiers);
    }

    virtual void dispatch_mouse_click(int x, int y, unsigned int buttons, unsigned int modifiers, unsigned int nth)
    {
        this->get_override("dispatch_mouse_click")(x, y, buttons, modifiers, nth);
    }

    virtual void dispatch_mouse_move(int x, int y, int dx, int dy, unsigned int buttons, unsigned int modifiers)
    {
        this->get_override("dispatch_mouse_move")(x, y, dx, dy, buttons, modifiers);
    }

    virtual void dispatch_mouse_up(int x, int y, unsigned int buttons, unsigned int modifiers)
    {
        this->get_override("dispatch_mouse_up")(x, y, buttons, modifiers);
    }

    virtual void dispatch_scroll(int x, int y, int scrollX, int scrollY)
    {
        this->get_override("dispatch_scroll")(x, y, scrollX, scrollY);
    }

    virtual void dispatch_resize(unsigned int w, unsigned int h)
    {
        this->get_override("dispatch_resize")(w, h);
    }

    virtual void dispatch_text_input(const char *text)
    {
        this->get_override("dispatch_text_input")(text);
    }

    virtual void dispatch_hide()
    {
        this->get_override("dispatch_hide")();
    }

    virtual void dispatch_show()
    {
        this->get_override("dispatch_show")();
    }

    virtual void dispatch_wm_quit()
    {
        this->get_override("dispatch_wm_quit")();
    }
};

void addWindowToInittab();

}

#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
