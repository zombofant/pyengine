/**********************************************************************
File name: Window.hpp
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
#ifndef _PYUNI_PYTHON_WINDOW_H
#define _PYUNI_PYTHON_WINDOW_H

#include <boost/python.hpp>
#include <string>

#include "WindowInterface/Display.hpp"
#include "WindowInterface/Window.hpp"
#include "WindowInterface/EventSink.hpp"
#include "WindowInterface/EventLoop.hpp"

namespace PyUni {

class DisplayWrap: public Display, public boost::python::wrapper<Display>
{
    virtual void selectMode(int index)
    {
        this->get_override("selectMode")(index);
    }

    virtual Window *createWindow(int w, int h, bool fullscreen=false)
    {
        return this->get_override("createWindow")(w, h, fullscreen);
    }
};

class WindowWrap: public Window, public boost::python::wrapper<Window>
{
    virtual void flip()
    {
        this->get_override("flip")();
    }

    virtual void switchTo()
    {
        this->get_override("switchTo")();
    }
};

class EventSinkWrap: public EventSink, public boost::python::wrapper<EventSink>
{
    virtual void frameSynced()
    {
        this->get_override("frameSynced")();
    }

    virtual void frameUnsynced(double deltaT)
    {
        this->get_override("frameUnsynced")(deltaT);
    }

    virtual void handleKeyDown(unsigned int symbol, unsigned int modifiers)
    {
        this->get_override("handleKeyDown")(symbol, modifiers);
    }

    virtual void handleKeyUp(unsigned int symbol, unsigned int modifiers)
    {
        this->get_override("handleKeyUp")(symbol, modifiers);
    }

    virtual void handleMouseDown(unsigned int x, unsigned int y, unsigned int buttons, unsigned int modifiers)
    {
        this->get_override("handleMouseDown")(x, y, buttons, modifiers);
    }

    virtual void handleMouseMove(unsigned int x, unsigned int y, int dx, int dy, unsigned int buttons, unsigned int modifiers)
    {
        this->get_override("handleMouseMove")(x, y, dx, dy, buttons, modifiers);
    }

    virtual void handleMouseUp(unsigned int x, unsigned int y, unsigned int buttons, unsigned int modifiers)
    {
        this->get_override("handleMouseUp")(x, y, buttons, modifiers);
    }

    virtual void handleMouseScroll(unsigned int x, unsigned int y, int scrollX, int scrollY)
    {
        this->get_override("handleMouseScroll")(x, y, scrollX, scrollY);
    }

    virtual void handleResize(unsigned int w, unsigned int h)
    {
        this->get_override("handleResize")(w, h);
    }

    virtual void handleTextInput(const char *text)
    {
        this->get_override("handleTextInput")(text);
    }

    virtual void handleHide()
    {
        this->get_override("handleHide")();
    }

    virtual void handleShow()
    {
        this->get_override("handleShow")();
    }

    virtual void handleWMQuit()
    {
        this->get_override("handleWMQuit")();
    }
};

void addWindowToInittab();

}

#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
