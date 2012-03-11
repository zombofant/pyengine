
#ifndef PYUNI_EVENT_SINK_H
#define PYUNI_EVENT_SINK_H

namespace PyUni {

class EventSink {
public:
    EventSink();
    virtual ~EventSink();

    virtual void onKeyDown(int symbol, int modifiers) = 0;
    virtual void onKeyUp(int symbol, int modifiers) = 0;
    virtual void onMouseDown(int x, int y, int buttons, int modifiers) = 0;
    virtual void onMouseMove(int x, int y, int dx, int dy,
                             int buttons, int modifiers) = 0;
    virtual void onMouseUp(int x, int y, int buttons, int modifiers) = 0;
    virtual void onResize() = 0;
    virtual void onScroll(int scrollx, int scrolly) = 0;
    virtual void onTextInput(const char *text) = 0;
    virtual void onCaretMotion(int motion) = 0;
    virtual void onCaretMotionSelect(int motion) = 0;
};

// for debug purposes
class DumpEventSink : public EventSink {
public:
    DumpEventSink();
    virtual ~DumpEventSink();

    virtual void onKeyDown(int symbol, int modifiers);
    virtual void onKeyUp(int symbol, int modifiers);
    virtual void onMouseDown(int x, int y, int buttons, int modifiers);
    virtual void onMouseMove(int x, int y, int dx, int dy,
                             int buttons, int modifiers);
    virtual void onMouseUp(int x, int y, int buttons, int modifiers);
    virtual void onResize();
    virtual void onScroll(int scrollx, int scrolly);
    virtual void onTextInput(const char *text);
    virtual void onCaretMotion(int motion);
    virtual void onCaretMotionSelect(int motion);
};

}

#endif

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
