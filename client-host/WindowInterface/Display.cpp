
#include <stdint.h>

#include "Display.h"
namespace PyUni {
Display::Display() {
}

void Display::normalizeScreenCoordinates() {
    int xmin = _screens[0].x, ymin = _screens[0].y;

    for (int i = 1; i < _screens.size(); i++) {
        if (_screens[i].x < xmin)
            xmin = _screens[i].x;

        if (_screens[i].y < ymin)
            ymin = _screens[i].y;
    }

    // normalize the coordinates, so that all origins will be positive
    for (int i = 0; i < _screens.size(); i++) {
        _screens[i].x -= xmin;
        _screens[i].y -= ymin;
    }
}

bool Display::hasDisplayMode(const DisplayMode &displayMode) {
    throw NotImplemented();
}
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
