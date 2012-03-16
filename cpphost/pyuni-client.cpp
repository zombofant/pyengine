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
#include <cassert>
#include <boost/python.hpp>

#include "WindowInterface/Display.hpp"
#include "WindowInterface/Window.hpp"
#include "WindowInterface/X11/X11Display.hpp"

#include "PythonInterface/Module.hpp"

#include <GL/gl.h>
#include <stdlib.h>
using namespace std;

PyUni::Display *disp = 0;

int main(int argc, char** argv) {
    try
    {
        PyUni::addToPython();
        Py_Initialize();
        PySys_SetArgv(argc, argv);


        boost::python::object main = boost::python::import("__main__");
        boost::python::object main_namespace = main.attr("__dict__");
        main_namespace["cuni"] = boost::python::import("cuni");

        PyUni::X11Display *x11 = new PyUni::X11Display();
        disp = x11;
        main_namespace["display"] = x11;

        exec("\
import time\n\
print(display)\n\
print('Available screens:')\n\
print('  '+'\\n  '.join(str(screen) for screen in display.Screens))\n\
displayModes = display.DisplayModes\n\
displayModes.sort(reverse=True)\n\
print('Available display modes:')\n\
print('  '+'\\n  '.join(str(dm) for dm in displayModes))\n\
print('Creating window with {0}'.format(displayModes[0]))\n\
win = display.createWindow(displayModes[0], 640, 480, False)\n\
win.switchTo()\n\
time.sleep(1)\n\
", main_namespace);
    }
    catch (boost::python::error_already_set const&)
    {
        PyErr_Print();
    }




    /*disp->dumpScreens();

    disp->selectMode(0);

    PyUni::Window *win = disp->createWindow(640, 480);

    win->switchTo();

    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    win->flip();

    sleep(5);*/

    return 0;
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:

