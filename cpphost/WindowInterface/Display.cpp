/**********************************************************************
File name: Display.cpp
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

#include <stdint.h>
#include <stdio.h>

#include "Display.hpp"
namespace PyUni {
Display::Display() {
}

Display::~Display() {
}

void Display::dumpScreens() {
    for (unsigned int i = 0; i < _screens.size(); i++) {
        printf("Screen %d rect(%d, %d, %d, %d)\n",
               i,
               _screens[i].x,
               _screens[i].y,
               _screens[i].width,
               _screens[i].height);
    }
}

void Display::normalizeScreenCoordinates() {
    int xmin = _screens[0].x, ymin = _screens[0].y;

    for (unsigned int i = 1; i < _screens.size(); i++) {
        if (_screens[i].x < xmin)
            xmin = _screens[i].x;

        if (_screens[i].y < ymin)
            ymin = _screens[i].y;
    }

    // normalize the coordinates, so that all origins will be positive
    for (unsigned int i = 0; i < _screens.size(); i++) {
        _screens[i].x -= xmin;
        _screens[i].y -= ymin;
    }
}

bool Display::hasDisplayMode(const DisplayMode &displayMode) {
    // todo
    return false;
}
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
