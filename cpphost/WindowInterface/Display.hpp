/**********************************************************************
File name: Display.hpp
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

#ifndef PYUNI_DISPLAY_H
#define PYUNI_DISPLAY_H

#include <vector>
#include <boost/smart_ptr>
namespace PyUni {
struct Screen {
    unsigned int index;
    bool primary;
    int x, y;
    unsigned int width, height;

    Screen(int posx,
           int posy,
           unsigned int aWidth,
           unsigned int aHeight,
           unsigned int aIndex,
           bool aPrimary):
        x(posx),
        y(posy),
        width(aWidth),
        height(aHeight),
        index(aIndex),
        primary(aPrimary)
    {

    }
};

struct DisplayMode {
    unsigned int redBits, greenBits, blueBits, alphaBits;
    unsigned int depthBits;
    unsigned int stencilBits;
    bool doubleBuffered;
    int index;

    DisplayMode(unsigned int aRedBits,
            unsigned int aGreenBits,
            unsigned int aBlueBits,
            unsigned int aAlphaBits,
            unsigned int aDepthBits,
            unsigned int aStencilBits,
            bool aDoubleBuffered,
            int aIndex):
        redBits(aRedBits),
        greenBits(aGreenBits),
        blueBits(aBlueBits),
        alphaBits(aAlphaBits),
        depthBits(aDepthBits),
        stencilBits(aStencilBits),
        doubleBuffered(aDoubleBuffered),
        index(aIndex)
    {

    }
};

class Display {
public:
    Display();
    virtual ~Display();
protected:
    void normalizeScreenCoordinates();

    std::vector<Screen> _screens;
    std::vector<DisplayMode> _displayModes;
public:
    bool hasDisplayMode(const DisplayMode &displayMode);
    void selectMode(int index);

    virtual Window createWindow(int w, int h, bool fullscreen=false) = 0;
};
}
#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
