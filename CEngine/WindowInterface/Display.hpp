/**********************************************************************
File name: Display.hpp
This file is part of: Pythonic Engine

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

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#ifndef _PYE_DISPLAY_H
#define _PYE_DISPLAY_H

#include <vector>
#include <iostream>

#include <boost/shared_ptr.hpp>

#include "CEngine/Misc/Exception.hpp"

#include "EventSink.hpp"

namespace PyEngine {

class DisplayError: public Exception {
public:
    DisplayError(const std::string message): Exception(message) {};
    DisplayError(const char *message): Exception(message) {};
};

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
        index(aIndex),
        primary(aPrimary),
        x(posx),
        y(posy),
        width(aWidth),
        height(aHeight)
    {

    }
};

struct DisplayMode {
    int redBits, greenBits, blueBits, alphaBits;
    int depthBits;
    int stencilBits;
    int samples;
    bool doubleBuffered;

    DisplayMode() = default;

    DisplayMode(int aRedBits,
            int aGreenBits,
            int aBlueBits,
            int aAlphaBits,
            int aDepthBits,
            int aStencilBits,
            int aSamples,
            bool aDoubleBuffered):
        redBits(aRedBits),
        greenBits(aGreenBits),
        blueBits(aBlueBits),
        alphaBits(aAlphaBits),
        depthBits(aDepthBits),
        stencilBits(aStencilBits),
        samples(aSamples),
        doubleBuffered(aDoubleBuffered)
    {

    }

    DisplayMode(const DisplayMode &ref):
        redBits(ref.redBits),
        greenBits(ref.greenBits),
        blueBits(ref.blueBits),
        alphaBits(ref.alphaBits),
        depthBits(ref.depthBits),
        stencilBits(ref.stencilBits),
        samples(ref.samples),
        doubleBuffered(ref.doubleBuffered)
    {

    }

    bool operator== (const DisplayMode &other) const;
    bool operator> (const DisplayMode &other) const;
    bool operator< (const DisplayMode &other) const;
    bool operator>= (const DisplayMode &other) const;
    bool operator<= (const DisplayMode &other) const;
    bool operator!= (const DisplayMode &other) const;
};


// forward declaration of the Window class
class Window;
typedef boost::shared_ptr<Window> WindowHandle;

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
    void dumpScreens();

    virtual WindowHandle createWindow(const DisplayMode &mode,
        int w, int h, bool fullscreen=false) = 0;
    virtual void pullEvents(EventSink *sink) = 0;

    const std::vector<Screen> &getScreens() const {
        return _screens;
    }

    const std::vector<DisplayMode> &getDisplayModes() const {
        return _displayModes;
    }

    void initializeGLEW() const;
    void finalizeGLEW() const;
};

typedef boost::shared_ptr<Display> DisplayHandle;

std::ostream& operator <<(std::ostream &stream, const Screen &screen);
std::ostream& operator <<(std::ostream &stream, const DisplayMode &dm);

}
#endif
// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
