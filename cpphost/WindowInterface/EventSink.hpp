/**********************************************************************
File name: EventSink.hpp
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
#ifndef _PYUNI_EVENTSINK_H
#define _PYUNI_EVENTSINK_H

#include <string>
#include <boost/shared_ptr.hpp>

namespace PyUni {

class EventSink {
    public:
        virtual void frameSynced() = 0;
        virtual void frameUnsynced(double deltaT) = 0;
        virtual void handleKeyDown(const std::string &symbol,
            unsigned int modifiers) = 0;
        virtual void handleKeyUp(const std::string &symbol,
            unsigned int modifiers) = 0;
        virtual void handleMouseDown(unsigned int x, unsigned int y,
            unsigned int buttons, unsigned int modifiers) = 0;
        virtual void handleMouseMove(unsigned int x, unsigned int y,
             int dx, int dy, unsigned int buttons,
             unsigned int modifiers) = 0;
        virtual void handleMouseUp(unsigned int x, unsigned int y,
            unsigned int buttons, unsigned int modifiers) = 0;
        virtual void handleMouseScroll(unsigned int x, unsigned int y,
            int scrollX, int scrollY) = 0;
        virtual void handleResize(unsigned int w, unsigned int h) = 0;
        virtual void handleTextInput(const std::string &text) = 0;
};

typedef boost::shared_ptr<EventSink> EventSinkHandle;

}

#endif
