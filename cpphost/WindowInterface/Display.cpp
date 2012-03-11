
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

bool Display::hasDisplayMode(const DisplayMode &displayMode) {
    return false;
}
}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
