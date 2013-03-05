/**********************************************************************
File name: Time.hpp
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
#ifndef _PYE_IO_TIME_H
#define _PYE_IO_TIME_H

#include <chrono>

namespace PyEngine {

// XXX: libstdc++ uses system_clock for this one, which I dislike,
// because especially on linux we have monotonic high-performance
// clocks -- but I cannot even switch, because on fedora 18,
// steady_clock is _not_ compiled into the libstdc++ (JWI)
typedef std::chrono::high_resolution_clock clock_to_use;

typedef double TimeFloat;
typedef std::chrono::time_point<clock_to_use> TimeStamp;

/* free functions */

void check_clock();
TimeStamp nanotime();
TimeFloat timeToDouble(const TimeStamp &time);
TimeFloat timeIntervalToDouble(const TimeStamp &prev, const TimeStamp &curr);

}

#endif
