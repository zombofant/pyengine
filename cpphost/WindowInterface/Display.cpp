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


bool DisplayMode::operator==(const DisplayMode &other) const
{
    return (
        redBits == other.redBits &&
        greenBits == other.greenBits &&
        blueBits == other.blueBits &&
        alphaBits == other.alphaBits &&
        depthBits == other.depthBits &&
        stencilBits == other.stencilBits &&
        doubleBuffered == other.doubleBuffered
    );
}
    
    
Display::Display() {
}

Display::~Display() {
}

void Display::dumpScreens() {
    for (unsigned int i = 0; i < _screens.size(); i++) {
        printf("Screen %d rect(%d, %d, %d, %d)\n", i, _screens[i].x, _screens[i].y, _screens[i].width, _screens[i].height);
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

int Display::findDisplayMode(const DisplayMode &displayMode) {
    for (unsigned int i = 0; i < _displayModes.size(); i++)
    {
        if (displayMode == _displayModes[i])
            return i;
    }
    return -1;
}


std::ostream& operator <<(std::ostream &stream, const Screen &screen)
{
    return stream <<
    "<Screen x=" << screen.x <<
        " y=" << screen.y <<
        " w=" << screen.width <<
        " h=" << screen.height <<
        " primary=" << screen.primary <<
    ">";
}

std::ostream& operator <<(std::ostream &stream, const DisplayMode &dm)
{
    return stream <<
    "<DisplayMode rgba=(" <<
            dm.redBits << "," <<
            dm.greenBits << "," <<
            dm.blueBits << "," <<
            dm.alphaBits <<
        ")" <<
        " depth=" << dm.depthBits <<
        " stencil=" << dm.stencilBits <<
        " doublebuf=" << dm.doubleBuffered <<
    ">";
}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
