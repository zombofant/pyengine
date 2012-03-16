#include "Module.hpp"
#include <vector>

#include "WindowInterface/X11/X11Display.hpp"
#include "WindowInterface/X11/X11Window.hpp"

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

BOOST_PYTHON_MODULE(cuni)
{
    class_<DisplayMode>("DisplayMode", init<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, bool>())
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
    class_<Screen>("Screen", no_init)
        .def_readonly("index", &Screen::index)
        .def_readonly("primary", &Screen::primary)
        .def_readonly("x", &Screen::x)
        .def_readonly("y", &Screen::y)
        .def_readonly("width", &Screen::width)
        .def_readonly("height", &Screen::height)
        .def(self_ns::repr(self))
    ;
    class_<WindowWrap, boost::noncopyable>("Window", no_init)
        .def("flip", pure_virtual(&Window::flip))
        .def("switchTo", pure_virtual(&Window::switchTo))
    ;
    class_<X11Window, bases<Window> >("X11Window", no_init);
    class_<DisplayWrap, boost::noncopyable>("Display", no_init)
        .add_property("Screens", &Display_screens_get)
        .add_property("DisplayModes", &Display_displayModes_get)
        .def("createWindow", pure_virtual(&Display::createWindow), boost::python::return_value_policy<boost::python::manage_new_object>())
    ;
    class_<X11Display, bases<Display> >("X11Display", no_init);
}

void addToPython() {
    PyImport_AppendInittab("cuni", &initcuni);
}

}
