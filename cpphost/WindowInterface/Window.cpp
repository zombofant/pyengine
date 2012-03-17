#include "Window.hpp"
#include <glew.h>

namespace PyUni {

/* PyUni::Window */

Window::Window():
    _glewInitialized(false)
{

}

Window::~Window()
{

}

void Window::initializeGLEW()
{
    if (_glewInitialized)
        return;
    glewInit();
    _glewInitialized = true;
}

}
