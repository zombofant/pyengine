/**********************************************************************
File name: Log.cpp
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
#include "Log.hpp"

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include "IO/Log.hpp"

#include "Helpers.hpp"

namespace PyUni {

using namespace boost::python;

void LogBase_log(LogBase *server, Severity severity, PyObject *text)
{
    const char *data;
    int len;
    PyObject *ref = extractUTF8String(text, &data, &len);

    server->log(severity, data);

    Py_XDECREF(ref);
}

void LogServer_log(LogServerHandle server, Severity severity, PyObject *text)
{
    LogBase_log(server.get(), severity, text);
}

LogChannelHandle LogServer_getChannel(LogServerHandle server, PyObject *text)
{
    const char *data;
    int len;
    PyObject *ref = extractUTF8String(text, &data, &len);

    LogChannelHandle handle = server->getChannel(data);

    Py_XDECREF(ref);
    return handle;
}

void LogChannel_log(LogChannelHandle channel, Severity severity, PyObject *text)
{
    LogBase_log(channel.get(), severity, text);
}

BOOST_PYTHON_MODULE(_cuni_log)
{
    enum_<Severity>("Severity")
        .value("Debug", Debug)
        .value("Information", Information)
        .value("Hint", Hint)
        .value("Warning", Warning)
        .value("Error", Error)
        .value("Panic", Panic)
    ;
    
    class_<LogChannel, LogChannelHandle, boost::noncopyable>("Channel", no_init)
        .def("log", &LogChannel_log)
        .add_property("Name", &LogChannel::getName)
    ;
    
    class_<LogServer, LogServerHandle, boost::noncopyable>("Server", no_init)
        .def("log", &LogServer_log)
        .def("addSink", &LogServer::addSink)
        .def("removeSink", &LogServer::removeSink)
        .def("getChannel", &LogServer_getChannel)
    ;

    class_<LogSink, LogSinkHandle, boost::noncopyable>("Sink", no_init)
    ;

    class_<LogStreamSink, bases<LogSink>, boost::shared_ptr<LogStreamSink>, boost::noncopyable>("StreamSink", init<uint64_t, StreamHandle>())
    ;

    class_<LogXMLSink, bases<LogSink>, boost::shared_ptr<LogXMLSink>, boost::noncopyable>("XMLSink", init<uint64_t, StreamHandle, std::string>())
    ;
}

void addLogToInittab()
{
    PyImport_AppendInittab("_cuni_log", &init_cuni_log);
}

}
