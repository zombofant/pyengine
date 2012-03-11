/**********************************************************************
File name: pyuni-client.cpp
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




#include <iostream>
//#include <boost/python.hpp>

#include "WindowInterface/Display.hpp"
#include "WindowInterface/Window.hpp"
#include "WindowInterface/X11/X11Display.hpp"

#include <GL/gl.h>
#include <stdlib.h>
using namespace std;
/*using namespace boost::python;

struct World
{
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg;
};

BOOST_PYTHON_MODULE(hello)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
    ;
}*/

int main() {


    /*PyImport_AppendInittab("hello", &inithello);
    Py_Initialize();
    object main = import("__main__");
    object main_namespace = main.attr("__dict__");

    exec(
"\
import hello;\
print(\"Hello World from inside-python print()!\");\
test = hello.World();\
test.set(\"Hello World from hello.World!\");\
print(test.greet());\
",
                          main_namespace);*/

    PyUni::Display *disp = new PyUni::X11Display();

    disp->dumpScreens();

    disp->selectMode(0);
    PyUni::Window *win = disp->createWindow(640, 480);

    win->switchTo();

    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    win->flip();

    sleep(10);

    return 0;
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:

