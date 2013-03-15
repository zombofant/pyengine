/**********************************************************************
File name: EventSink.hpp
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
#ifndef _PYE_EVENTSINK_H
#define _PYE_EVENTSINK_H

#include <string>

#include <boost/shared_ptr.hpp>

namespace PyEngine {

class EventSink {
public:
    virtual ~EventSink();
    virtual void frame_synced() = 0;
    virtual void frame_unsynced(double deltaT) = 0;
    virtual void dispatch_key_down(unsigned int key,
                                   unsigned int modifiers) = 0;
    virtual void dispatch_key_up(unsigned int key,
                                 unsigned int modifiers) = 0;
    virtual void dispatch_mouse_down(int x, int y,
                                     unsigned int buttons,
                                     unsigned int modifiers) = 0;
    virtual void dispatch_mouse_click(int x, int y,
                                      unsigned int buttons,
                                      unsigned int modifiers,
                                      unsigned int nth) = 0;
    virtual void dispatch_mouse_move(int x, int y,
                                     int dx, int dy,
                                     unsigned int buttons,
                                     unsigned int modifiers) = 0;
    virtual void dispatch_mouse_up(int x, int y,
                                   unsigned int buttons,
                                   unsigned int modifiers) = 0;
    virtual void dispatch_scroll(int x, int y,
                                 int scrollX, int scrollY) = 0;
    virtual void dispatch_resize(unsigned int w, unsigned int h) = 0;
    virtual void dispatch_text_input(const char *text) = 0;
    virtual void dispatch_hide() = 0;
    virtual void dispatch_show() = 0;
    virtual void dispatch_wm_quit() = 0;
};

typedef boost::shared_ptr<EventSink> EventSinkHandle;

}

#endif

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
