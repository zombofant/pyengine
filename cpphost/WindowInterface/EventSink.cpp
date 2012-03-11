
#include "EventSink.hpp"
#include <stdio.h>

namespace PyUni {

EventSink::EventSink() {}

EventSink::~EventSink() {}


DumpEventSink::DumpEventSink() {}

DumpEventSink::~DumpEventSink() {}

void DumpEventSink::onKeyDown(int symbol, int modifiers) {

}

void DumpEventSink::onKeyUp(int symbol, int modifiers) {

}

void DumpEventSink::onMouseDown(int x, int y, int buttons, int modifiers) {

}

void DumpEventSink::onMouseMove(int x, int y, int dx, int dy,
                             int buttons, int modifiers) {

}

void DumpEventSink::onMouseUp(int x, int y, int buttons, int modifiers) {

}

void DumpEventSink::onResize() {

}

void DumpEventSink::onScroll(int scrollx, int scrolly) {

}

void DumpEventSink::onTextInput(const char *text) {

}

void DumpEventSink::onCaretMotion(int motion) {

}

void DumpEventSink::onCaretMotionSelect(int motion) {

}

}

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
