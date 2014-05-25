/**********************************************************************
File name: Thread.hpp
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

#ifndef _PYE_IO_THREAD_H
#define _PYE_IO_THREAD_H

#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

namespace PyEngine {

class Semaphore {
public:
    typedef unsigned int value_type;

public:
    Semaphore(const value_type value = 0);

private:
    std::mutex _cv_lock;
    std::condition_variable _cv;
    value_type _value;
    std::function<bool(void)> _checker;

private:
    bool check_and_decrease();

public:
    void post(const value_type amount = 1);
    void wait();

    template <typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period> &rel_time)
    {
        std::unique_lock<std::mutex> lock;
        return _cv.wait_for(
            lock,
            rel_time,
            _checker);
    }

    template <typename Clock, typename Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration> &timeout_time)
    {
        std::unique_lock<std::mutex> lock;
        return _cv.wait_until(
            lock,
            timeout_time,
            _checker);
    }

};

unsigned int get_hardware_thread_count();

}

#endif
