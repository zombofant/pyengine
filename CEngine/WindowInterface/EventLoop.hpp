/**********************************************************************
File name: EventLoop.hpp
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
#ifndef _PYUNI_WINDOW_EVENTLOOP_H
#define _PYUNI_WINDOW_EVENTLOOP_H

#include "IO/Time.hpp"

#include "EventSink.hpp"
#include "Display.hpp"

namespace PyUni {

class EventLoop {
    public:
        EventLoop(DisplayHandle display, EventSinkHandle eventSink);
        virtual ~EventLoop();
    private:
        DisplayHandle _display;
        EventSinkHandle _eventSink;
        TimeFloat _deltaT;
        uint64_t _frameCount;
        uint64_t _currentFrameCount;
        bool _terminated;
    public:
        void terminate();
        void run();
    public:
        double getSyncedFrameLength();
        void setSyncedFrameLength(const TimeFloat deltaT);

        /**
         * Set a frame count. Until this count of further frames, the
         * event loop will send a WMQuit event.
         */
        void setFrameCount(const uint64_t frameCount);
    public:
        double currentFPS;
};


}

#endif
