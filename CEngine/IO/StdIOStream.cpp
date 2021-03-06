/**********************************************************************
File name: StdIOStream.cpp
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
#include "StdIOStream.hpp"

#include <unistd.h>

namespace PyEngine {

/* PyEngine::StdIOStream */

StdIOStream::StdIOStream(const int origFD):
    FDStream::FDStream(checkFD(dup(origFD)), true)
{
    
}

bool StdIOStream::isSeekable() const {
    return false;
}

/* PyEngine::StdInStream */

StdInStream::StdInStream():
    StdIOStream::StdIOStream(STDIN_FILENO)
{
    
}

bool StdInStream::isReadable() const {
    return true;
}

bool StdInStream::isWritable() const {
    return false;
}

/* PyEngine::StdOutStream */

StdOutStream::StdOutStream():
    StdIOStream::StdIOStream(STDOUT_FILENO)
{
    
}

bool StdOutStream::isReadable() const {
    return false;
}

bool StdOutStream::isWritable() const {
    return true;
}

/* PyEngine::StdErrStream */

StdErrStream::StdErrStream():
    StdIOStream::StdIOStream(STDERR_FILENO)
{
    
}

bool StdErrStream::isReadable() const {
    return false;
}

bool StdErrStream::isWritable() const {
    return true;
}

StreamHandle stdin(new StdInStream()), stdout(new StdOutStream()), stderr(new StdErrStream());

}
