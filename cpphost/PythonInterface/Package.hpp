/**********************************************************************
File name: Package.hpp
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
#ifndef _PYUNI_PYTHON_PACKAGE_H
#define _PYUNI_PYTHON_PACKAGE_H

#include "IO/Stream.hpp"
#include <boost/python.hpp>

namespace PyUni {

class PyStream;
typedef boost::shared_ptr<PyStream> PyStreamHandle;

class PyStream: public Stream
{
    public:
        PyStream(PyObject *obj);
        ~PyStream() throw();
    private:
        PyObject *_readCall, *_writeCall, *_seekCall, *_flushCall,
            *_tellCall;
        const bool _readable, _seekable, _writeable;
    protected:
        void doSeek(const int whence, const sizeint offset) const;
    public:
        virtual void flush();
        virtual sizeuint read(void *data, const sizeuint length);
        virtual sizeuint seek(const int whence, const sizeint offset);
        virtual const sizeuint size() const;
        virtual const sizeuint tell() const;
        virtual sizeuint write(const void *data, const sizeuint length);
    public:
        virtual bool isReadable() const { return _readable; };
        virtual bool isSeekable() const { return _seekable; };
        virtual bool isWritable() const { return _writeable; };
    public:
        static PyStreamHandle create(boost::python::object obj);
};

void addCUniToInittab();

}

#endif 
