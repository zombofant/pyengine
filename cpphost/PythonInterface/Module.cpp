#include "Module.hpp"
#include "WindowInterface/Display.hpp"
#include <boost/python.hpp>
#include <vector>
#include "WindowInterface/X11/X11Display.hpp"

using namespace boost::python;

namespace PyUni {

PyObject *Display_screens_get(const Display &self)
{
    const std::vector<Screen> &screens = self.getScreens();
    PyObject *pyList = PyList_New(screens.size());
    list boostList = list(handle<>(pyList));
    for (unsigned int i = 0; i < screens.size(); i++)
    {
        boostList[i] = screens[i];
    }
    return pyList;
}

PyObject *Display_displayModes_get(const Display &self)
{
    const std::vector<Screen> &displayModes = self.getDisplayModes();
    PyObject *pyList = PyList_New(displayModes.size());
    list boostList = list(handle<>(pyList));
    for (unsigned int i = 0; i < displayModes.size(); i++)
    {
        boostList[i] = displayModes[i];
    }
    return pyList;
}

BOOST_PYTHON_MODULE(cuni)
{
    class_<DisplayMode>("DisplayMode", init<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, bool>())
        .def_readwrite("redBits", &DisplayMode::redBits)
        .def_readwrite("greenBits", &DisplayMode::greenBits)
        .def_readwrite("blueBits", &DisplayMode::blueBits)
        .def_readwrite("alphaBits", &DisplayMode::alphaBits)
        .def_readwrite("depthBits", &DisplayMode::depthBits)
        .def_readwrite("stencilBits", &DisplayMode::stencilBits)
        .def_readwrite("doubleBuffered", &DisplayMode::doubleBuffered)
        .def_readonly("index", &DisplayMode::index)
    ;
    class_<Screen>("Screen", no_init)
        .def_readonly("index", &Screen::index)
        .def_readonly("primary", &Screen::primary)
        .def_readonly("x", &Screen::x)
        .def_readonly("y", &Screen::y)
        .def_readonly("width", &Screen::width)
        .def_readonly("height", &Screen::height)
    ;
    class_<Display, boost::noncopyable>("Display", no_init)
        .addProperty("Screens", &Display_screens_get)
        .addProperty("DisplayModes", &Display_displayModes_get)
}

void addToPython() {
    PyImport_AppendInittab("cuni", &initcuni);
}

}
