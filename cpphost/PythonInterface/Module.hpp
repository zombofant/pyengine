#ifndef _PYUNI_PYTHON_MODULE_H
#define _PYUNI_PYTHON_MODULE_H

#include "WindowInterface/Display.hpp"
#include "WindowInterface/Window.hpp"
#include <boost/python.hpp>

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

void addToPython();

}

#endif
