/**********************************************************************
File name: Thread.cpp
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

#include "Thread.hpp"

#include <thread>

#include "CEngine/IO/Log.hpp"

namespace PyEngine {

static bool _hardware_thread_count_known = false;
static unsigned int _hardware_thread_count = 0;

/* PyEngine::Semaphore */

Semaphore::Semaphore(const unsigned int value):
    _cv_lock(),
    _cv(),
    _value(value),
    _checker(std::bind(&Semaphore::check_and_decrease, this))
{

}

bool Semaphore::check_and_decrease()
{
    if (_value) {
        --_value;
        return true;
    }
    return false;
}

void Semaphore::post(const value_type amount)
{
    std::lock_guard<std::mutex> lock(_cv_lock);
    _value += amount;
    _cv.notify_all();
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(_cv_lock);
    while (!_value) {
        _cv.wait(lock);
    }
    --_value;
}

unsigned int get_hardware_thread_count()
{
    if (_hardware_thread_count_known)
        return _hardware_thread_count;

    unsigned int count = std::thread::hardware_concurrency();
    if (count == 0) {
        // lets fall back to platform-dependent shit
        // TODO: win & macos stuff, see source here:
        // https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
        log->getChannel("system")->logf(
            Hint,
            "Falling back to custom methods for hardware thread count detection");
        count = sysconf(_SC_NPROCESSORS_ONLN);
    }

    if (count == 0) {
        log->getChannel("system")->logf(
            Warning,
            "Unknown amount of hardware threads, falling back to 1.");
        count = 1;
    }

    _hardware_thread_count_known = true;
    _hardware_thread_count = count;
    log->getChannel("system")->log(Information)
        << "Detected " << count << " hardware threads." << submit;

    return count;
}

}
