
#include <stdint.h>

#include "Display.h"
namespace PyUni {
Display::Display() {
}

void Display::normalizeScreenCoordinates() {
    int xmin = screens[0].x, ymin = screens[0].y;

    for (int i = 1; i < screens.size(); i++) {
        if (screens[i].x < xmin)
            xmin = screens[i].x;

        if (screens[i].y < ymin)
            ymin = screens[i].y;
    }

    // normalize the coordinates, so that all origins will be positive
    for (int i = 0; i < screens.size(); i++) {
        screens[i].x -= xmin;
        screens[i].y -= ymin;
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
