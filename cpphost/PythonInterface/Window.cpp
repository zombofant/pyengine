/**********************************************************************
File name: Window.cpp
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
#include <vector>
#include <boost/shared_ptr.hpp>

#include "WindowInterface/X11/X11Display.hpp"
#include "WindowInterface/X11/X11Window.hpp"

#include "Window.hpp"

using namespace boost::python;

namespace PyUni {

list Display_screens_get(const Display &self)
{
    const std::vector<Screen> &screens = self.getScreens();
    list boostList = list();
    for (unsigned int i = 0; i < screens.size(); i++)
    {
        boostList.append(screens[i]);
    }
    return boostList;
}

list Display_displayModes_get(const Display &self)
{
    const std::vector<DisplayMode> &displayModes = self.getDisplayModes();
    list boostList = list();
    for (unsigned int i = 0; i < displayModes.size(); i++)
    {
        boostList.append(displayModes[i]);
    }
    return boostList;
}

BOOST_PYTHON_MODULE(_cuni_window)
{
    class_<DisplayMode, boost::shared_ptr<DisplayMode> >("DisplayMode", init<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, bool>())
        .def_readwrite("redBits", &DisplayMode::redBits)
        .def_readwrite("greenBits", &DisplayMode::greenBits)
        .def_readwrite("blueBits", &DisplayMode::blueBits)
        .def_readwrite("alphaBits", &DisplayMode::alphaBits)
        .def_readwrite("depthBits", &DisplayMode::depthBits)
        .def_readwrite("stencilBits", &DisplayMode::stencilBits)
        .def_readwrite("doubleBuffered", &DisplayMode::doubleBuffered)
        .def(self < other<DisplayMode>())
        .def(self <= other<DisplayMode>())
        .def(self > other<DisplayMode>())
        .def(self >= other<DisplayMode>())
        .def(self == other<DisplayMode>())
        .def(self != other<DisplayMode>())
        .def(self_ns::repr(self))
    ;

    class_<Screen, boost::shared_ptr<Screen> >("Screen", no_init)
        .def_readonly("index", &Screen::index)
        .def_readonly("primary", &Screen::primary)
        .def_readonly("x", &Screen::x)
        .def_readonly("y", &Screen::y)
        .def_readonly("width", &Screen::width)
        .def_readonly("height", &Screen::height)
        .def(self_ns::repr(self))
    ;

    /*class_<WindowWrap, WindowHandle, boost::noncopyable>("Window", no_init)
        .def("flip", pure_virtual(&Window::flip))
        .def("switchTo", pure_virtual(&Window::switchTo))
    ;*/
    class_<Window, WindowHandle, boost::noncopyable>("Window", no_init)
        .def("flip", pure_virtual(&Window::flip))
        .def("switchTo", pure_virtual(&Window::switchTo))
    ;
    class_<X11Window, bases<Window> >("X11Window", no_init)
        .def("flip", &X11Window::flip)
        .def("switchTo", &X11Window::switchTo)
    ;;

    class_<DisplayWrap, DisplayHandle, boost::noncopyable>("Display", no_init)
        .add_property("Screens", &Display_screens_get)
        .add_property("DisplayModes", &Display_displayModes_get)
        .def("createWindow", pure_virtual(&Display::createWindow))
    ;
    class_<X11Display, bases<Display> >("X11Display", no_init);

    implicitly_convertible<boost::shared_ptr<DisplayWrap>, DisplayHandle >();
    
    class_<EventSinkWrap, boost::shared_ptr<EventSinkWrap>, boost::noncopyable>("EventSink")
        .def("frameSynced", pure_virtual(&EventSink::frameSynced))
        .def("frameUnsynced", pure_virtual(&EventSink::frameUnsynced))
        .def("handleKeyDown", pure_virtual(&EventSink::handleKeyDown))
        .def("handleKeyUp", pure_virtual(&EventSink::handleKeyUp))
        .def("handleMouseDown", pure_virtual(&EventSink::handleMouseDown))
        .def("handleMouseMove", pure_virtual(&EventSink::handleMouseMove))
        .def("handleMouseUp", pure_virtual(&EventSink::handleMouseUp))
        .def("handleMouseScroll", pure_virtual(&EventSink::handleMouseScroll))
        .def("handleResize", pure_virtual(&EventSink::handleResize))
        .def("handleTextInput", pure_virtual(&EventSink::handleTextInput))
    ;
    // class_<EventSinkWrap, bases<EventSink>, boost::shared_ptr<EventSinkWrap> >("EventSink");
    
    boost::python::implicitly_convertible<boost::shared_ptr<EventSinkWrap>, EventSinkHandle >();
    
    class_<EventLoop, boost::shared_ptr<EventLoop>, boost::noncopyable>("EventLoop", init<DisplayHandle, EventSinkHandle>())
        .def("run", &EventLoop::run)
        .add_property("SyncedFrameLength", &EventLoop::getSyncedFrameLength, &EventLoop::setSyncedFrameLength)
    ;
}

void addWindowToInittab() {
    PyImport_AppendInittab("_cuni_window", &init_cuni_window);
}

}
