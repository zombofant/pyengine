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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/

#ifndef _PYE_IO_THREAD_H
#define _PYE_IO_THREAD_H

#include <semaphore.h>

#include "CEngine/Misc/Exception.hpp"

namespace PyEngine {

class PThreadError: public ExternalError {
public:
    PThreadError(const std::string &message): ExternalError("pthread", message.c_str()) {};
    PThreadError(const char *message): ExternalError("pthread", message) {};
    PThreadError(const char *functionName, const int error);
};

class Semaphore {
public:
    Semaphore(const unsigned int value = 0);
    ~Semaphore();
private:
    sem_t _sem;
public:
    void post();
    bool tryWait();
    void wait();
};

class Thread {
public:
    Thread();
    virtual ~Thread();
private:
    Semaphore _resume;
    pthread_t _thread;
    bool _deleteOnTerminate;
protected:
    void suspend();
public:
    bool getDeleteOnTerminate() { return _deleteOnTerminate; };
    void setDeleteOnTerminate(bool value) { _deleteOnTerminate = value; };
public:
    virtual void *execute();
    void *main();
    void resume();
public:
    static unsigned int getHardwareThreadCount();
};

class Mutex {
public:
    Mutex();
    ~Mutex();
private:
    pthread_mutex_t _mutex;
public:
    void lock();
    bool tryLock();
    void unlock();
};

}

#endif
