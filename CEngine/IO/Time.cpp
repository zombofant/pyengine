/**********************************************************************
File name: Time.cpp
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
#include "Time.hpp"

#include "CEngine/IO/Log.hpp"

namespace PyEngine {

#ifdef _GLIBCXX_USE_CLOCK_MONOTONIC
// libstdc++ uses, for an unknown reason, clock_gettime(CLOCK_REALTIME)
// instead of clock_gettime(CLOCK_MONOTONIC) for the
// high_resolution_clock.
typedef std::chrono::steady_clock clock_to_use;
#else
typedef std::chrono::high_resolution_clock clock_to_use;
#endif

typedef std::chrono::duration<double> HiResDuration;


/* free functions */

void check_clock() {
    if (!clock_to_use::is_steady) {
        log->getChannel("time")->log(Warning) << "clock_to_use is not monotonous." << submit;
    }
}

TimeStamp nanotime() {
    return clock_to_use::now();
}

TimeFloat timeToDouble(const TimeStamp &time) {
    return std::chrono::duration_cast<HiResDuration>(
        time.time_since_epoch()
        ).count();
}

TimeFloat timeIntervalToDouble(const TimeStamp &prev, const TimeStamp &curr) {
    return std::chrono::duration_cast<HiResDuration>(
        curr - prev
        ).count();
}

}
