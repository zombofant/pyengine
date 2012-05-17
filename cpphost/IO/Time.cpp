/**********************************************************************
File name: Time.cpp
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
#include "Time.hpp"

namespace PyUni {

/* free functions */

timespec nanotime() {
    timespec result;
    clock_gettime(CLOCK_MONOTONIC_RAW, &result);
    return result;
}

TimeFloat timeToDouble(const timespec &time) {
    return TimeFloat(time.tv_sec) + TimeFloat(time.tv_nsec) / 1000000000.;
}

TimeFloat timeIntervalToDouble(const timespec &prev, const timespec &curr) {
    TimeFloat result = (curr.tv_sec - prev.tv_sec);
    result += TimeFloat(curr.tv_nsec - prev.tv_nsec) / (1000000000.);
    return result;
}

}
