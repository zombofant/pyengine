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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/

#include "Thread.hpp"

#include <thread>

#include <boost/format.hpp>

#include "CEngine/IO/Log.hpp"

namespace PyEngine {

void *bootstrap(void *thread)
{
    return ((Thread*)thread)->main();
}

void cleanup(void *thread)
{
    Thread * const threadObj = (Thread*)thread;
    if (threadObj->getDeleteOnTerminate())
        delete threadObj;
}

/* PyEngine::PThreadError */

PThreadError::PThreadError(const char *functionName, const int error):
    ExternalError::ExternalError("pthread", (boost::format("%s returned error code %d") % functionName % error).str().c_str())
{

}

/* PyEngine::Thread */

Thread::Thread():
    _resume(),
    _thread(),
    _deleteOnTerminate(true)
{
    int error = pthread_create(&_thread, 0, &bootstrap, this);
    if (error != 0) {
        throw PThreadError("pthread_create", error);
    }
}

Thread::~Thread()
{
    std::cout << "destroying thread" << std::endl;
}

void Thread::suspend()
{
    _resume.wait();
}

void *Thread::execute()
{
    return 0;
}

void *Thread::main()
{
    void *retval = 0;
    pthread_cleanup_push(&cleanup, this);
    try {
        retval = execute();
    } catch (Exception const& err) {
        std::cerr << err.what() << std::endl;
        log->logException(err, Panic);
    }
    pthread_cleanup_pop(1);
    return retval;
}

void Thread::resume()
{
    _resume.post();
}

unsigned int Thread::getHardwareThreadCount()
{
    if (_hardwareThreadCountKnown)
        return _hardwareThreadCount;

    unsigned int count = std::thread::hardware_concurrency();
    if (count == 0) {
        // lets fall back to platform-dependent shit
        // TODO: win & macos stuff, see source here:
        // http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
        log->getChannel("system")->logf(Hint, "Falling back to custom methods for hardware thread count detection");
        count = sysconf(_SC_NPROCESSORS_ONLN);
    }

    if (count == 0) {
        log->getChannel("system")->logf(Warning, "Unknown amount of hardware threads, falling back to 1.");
        count = 1;
    }

    _hardwareThreadCountKnown = true;
    _hardwareThreadCount = count;
    log->getChannel("system")->log(Information) << "Detected " << count << " hardware threads." << submit;
    return count;
}

/* PyEngine::Mutex */

Mutex::Mutex():
    _mutex()
{
    int error = 0;
    error = pthread_mutex_init(&_mutex, 0);
    if (error != 0) {
        throw PThreadError("pthread_mutex_init", error);
    }
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&_mutex);
}

void Mutex::lock()
{
    const int status = pthread_mutex_lock(&_mutex);
    if (status == 0) return;
    throw PThreadError("pthread_mutex_lock", status);
}

bool Mutex::tryLock()
{
    const int status = pthread_mutex_trylock(&_mutex);
    if (status == EBUSY) {
        return false;
    }
    assert(status == 0);
    return true;
}

void Mutex::unlock()
{
    const int status = pthread_mutex_unlock(&_mutex);
    if (status == 0) return;
    throw PThreadError("pthread_mutex_unlock", status);
}

/* PyEngine::Semaphore */

Semaphore::Semaphore(const unsigned int value):
    _sem()
{
    if (sem_init(&_sem, 0, value)) {
        raiseLastOSError();
    }
}

Semaphore::~Semaphore()
{
    sem_destroy(&_sem);
}

void Semaphore::post()
{
    assert(sem_post(&_sem) == 0);
}

bool Semaphore::tryWait()
{
    if (sem_trywait(&_sem)) {
        const int error = errno;
        if (error == EAGAIN) {
            return false;
        }
        raiseLastOSError();
    }
    return true;
}

void Semaphore::wait()
{
    assert(sem_wait(&_sem) == 0);
}


bool Thread::_hardwareThreadCountKnown = false;
unsigned int Thread::_hardwareThreadCount = 0;

}
