/**********************************************************************
File name: EventLoop.cpp
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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#include "EventLoop.hpp"

#include <iostream>

#include <boost/python.hpp>

#include "IO/Log.hpp"

#include "Misc/Exception.hpp"
#include "Display.hpp"

namespace PyEngine {

EventLoop::EventLoop(DisplayHandle display, EventSinkHandle eventSink):
    _display(display),
    _eventSink(eventSink),
    _deltaT(0.01),
    _frameCount(0),
    _currentFrameCount(0),
    _terminated(false),
    currentFPS(0.0)
{
    
}

EventLoop::~EventLoop()
{
    throw Exception("Being destructed");
}

double EventLoop::getSyncedFrameLength()
{
    return _deltaT;
}

void EventLoop::setSyncedFrameLength(const TimeFloat deltaT)
{
    _deltaT = deltaT;
}

void EventLoop::setFrameCount(const uint64_t frameCount)
{
    _frameCount = frameCount;
    _currentFrameCount = 0;
}

void EventLoop::terminate()
{
    _terminated = true;
}

void EventLoop::run()
{
    timespec lastUpdate, frameCounterStart;
    Display *display = _display.get();
    EventSink *eventSink = _eventSink.get();
    uint32_t frameCount = 0;
    TimeFloat accumulatedTime = 0.;
    try {
        lastUpdate = nanotime();
        frameCounterStart = lastUpdate;
        while (!_terminated) {
            display->pullEvents(eventSink);
            
            const timespec currentUpdate = nanotime();
            const TimeFloat interval = timeIntervalToDouble(lastUpdate, currentUpdate);
            const TimeFloat fpsInterval = timeIntervalToDouble(frameCounterStart, currentUpdate);
            
            if (fpsInterval >= 1.0) {
                currentFPS = (double)(frameCount) / fpsInterval;
                std::cout << "fps: " << currentFPS << std::endl;
                frameCounterStart = currentUpdate;
                frameCount = 0;
            }
            
            if (interval >= 0.01) {
                accumulatedTime += interval;
                const TimeFloat frameLength = _deltaT;
                while (accumulatedTime >= frameLength) {
                    accumulatedTime -= frameLength;
                    eventSink->frameSynced();
                }
                
                lastUpdate = currentUpdate;
                eventSink->frameUnsynced(interval);
                frameCount++;
                if (_frameCount > 0) {
                    _currentFrameCount++;
                    if (_currentFrameCount >= _frameCount) {
                        log->log(Information, "Frame count limit reached, sending WMQuit");
                        eventSink->handleWMQuit();
                    }
                }
            } else {
                usleep(1);
            }
        }
    } catch (boost::python::error_already_set) {
        std::cerr << "During mainloop call-in: " << std::endl;
        throw;
    } catch (std::exception e) {
        std::cerr << e.what() << " during runtime." << std::endl;
        throw;
    } catch (...) {
        std::cerr << "unknown exception during runtime." << std::endl;
        throw;
    }
}

}
