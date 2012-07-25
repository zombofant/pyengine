/**********************************************************************
File name: Log.cpp
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
#include "Log.hpp"

#include <cstdio>
#include <cstdarg>

#include "CEngine/Misc/Int.hpp"

namespace PyEngine {

const sizeint AWESOME_BUFFER_SIZE = 512;

using namespace std;

char *awesomef(const char *message, va_list args, sizeuint *length = 0)
{
    char *buffer = (char*)malloc(AWESOME_BUFFER_SIZE);
    sizeint actualLength = vsnprintf(buffer, AWESOME_BUFFER_SIZE, message, args);
    if (actualLength >= AWESOME_BUFFER_SIZE) {
        buffer = (char*)realloc(buffer, actualLength+1);
        assert(vsnprintf(buffer, actualLength+1, message, args) == actualLength);
    } else {
        buffer = (char*)realloc(buffer, actualLength+1);
    }
    if (length)
        *length = actualLength;
    return buffer;
}

char *vawesomef(const char *message, sizeuint *length, ...)
{
    va_list args;
    va_start(args, length);
    char *result = awesomef(message, args, length);
    va_end(args);
    return result;
}

/* PyEngine::IO::LogPipe */

LogPipe::LogPipe(LogServer *server, Severity severity, LogChannel *channel):
    std::ostringstream(ios_base::app | ios_base::out),
    _server(server),
    _severity(severity),
    _channel(channel)
{
    
}

LogPipe::~LogPipe()
{

}

void LogPipe::submit()
{
    _server->log(this);
}

/* PyEngine::IO::LogChannel */

LogChannel::LogChannel(LogServer *server, const std::string name):
    _server(server),
    _name(name)
{
    
}

void LogChannel::log(Severity severity, const char *message)
{
    _server->log(severity, this, message);
}

void LogChannel::logf(Severity severity, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    char *formatted = awesomef(message, args);
    va_end(args);
    _server->log(severity, this, formatted);
    free(formatted);
}

LogPipe &LogChannel::log(Severity severity)
{
    return _server->log(severity, this);
}

/* PyEngine::IO::LogSink */

LogSink::LogSink(uint64_t mask):
    _mask(mask)
{

}

void LogSink::log(TimeFloat timestamp, Severity severity,
    LogChannel *channel, const char *message)
{
    if ((severity & _mask) != 0)
    {
        doLog(timestamp, severity, channel, message);
    }
}

/* PyEngine::IO::LogStreamSink */

LogStreamSink::LogStreamSink(uint64_t mask, StreamHandle stream):
    LogSink::LogSink(mask),
    _streamHandle(stream),
    _stream(stream.get())
{
    
}

void LogStreamSink::doLog(TimeFloat timestamp, Severity severity,
    LogChannel *channel, const char *message)
{
    const char *severityName = SeverityName(severity);
    sizeuint length = 0;
    char *formatted = vawesomef("[%12.4f] [%s] [%s] %s\n", &length, timestamp, severityName, channel->getName(), message);
    _stream->write(formatted, length);
    free(formatted);
    _stream->flush();
}

/* PyEngine::IO::LogTTYSink */

LogTTYSink::LogTTYSink(uint64_t mask, StreamHandle stream):
    LogStreamSink::LogStreamSink(mask, stream)
{

}

void LogTTYSink::doLog(TimeFloat timestamp, Severity severity,
    LogChannel *channel, const char *message)
{
    const char *severityName = SeverityName(severity);
    const char *severityANSI = SeverityANSI(severity);
    sizeuint length = 0;
    char *formatted = vawesomef("%s[%12.4f] [%5s]\033[0m [%s] %s\n", &length, severityANSI, timestamp, severityName, channel->getName(), message);
    _stream->write(formatted, length);
    free(formatted);
    _stream->flush();
}
    

/* PyEngine::IO::LogXMLSink */

LogXMLSink::LogXMLSink(uint64_t mask, StreamHandle stream,
        const std::string xsltFile, const std::string branding):
    LogSink::LogSink(mask),
    _streamHandle(stream),
    _stream(stream.get())
{
    _stream->writeNullTerminated("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<?xml-stylesheet type=\"text/xsl\" href=\"");
    _stream->writeNullTerminated(xsltFile.c_str());
    _stream->writeNullTerminated("\"?>\n\
<log>\
<branding><application-name>");
    _stream->writeNullTerminated(branding.c_str());
    _stream->writeNullTerminated("</application-name></branding>\n");
}

LogXMLSink::~LogXMLSink()
{
    _stream->writeNullTerminated("</log>");
    _stream->flush();
}

void LogXMLSink::doLog(TimeFloat timestamp, Severity severity,
    LogChannel *channel, const char *message)
{
    sizeuint length = 0;
    char *formatted = vawesomef("<message>\
<timestamp>%f</timestamp>\
<severity>%s</severity>\
<channel>%s</channel>\
<text><![CDATA[%s]]></text>\
</message>\n", &length, timestamp, SeverityName(severity), channel->getName(), message);
    _stream->write(formatted, length);
    _stream->flush();
    free(formatted);
}

/* PyEngine::IO::LogServer */

LogServer::LogServer():
    _startupTime(nanotime()),
    _sinks(),
    _channels(),
    _globalChannel(getChannel("global"))
{
    
}

void LogServer::log(Severity severity, LogChannel *channel, const char *message)
{
    TimeFloat timestamp = timeIntervalToDouble(_startupTime, nanotime());
    for (auto it = _sinks.begin(); it != _sinks.end(); it++)
    {
        LogSink *sink = (*it).get();
        sink->log(timestamp, severity, channel, message);
    }
}

void LogServer::logf(Severity severity, LogChannel *channel, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    char *formatted = awesomef(message, args);
    va_end(args);
    log(severity, channel, formatted);
    free(formatted);
}

LogPipe &LogServer::log(Severity severity, LogChannel *channel)
{
    LogPipe *stream = new LogPipe(this, severity, channel);
    return *stream;
}

void LogServer::log(LogPipe *stream)
{
    log(stream->_severity, stream->_channel, stream->str().c_str());
    delete stream;
}

void LogServer::addSink(LogSinkHandle sink)
{
    _sinks.push_back(sink);
}

LogChannelHandle LogServer::getChannel(const std::string &name)
{
    LogChannelHandle channel(new LogChannel(this, name));
    _channels[name] = channel;
    return channel;
}
 
void LogServer::removeSink(LogSinkHandle sink)
{
    _sinks.remove(sink);
}

void LogServer::log(Severity severity, const char *message)
{
    log(severity, _globalChannel.get(), message);
}

void LogServer::logf(Severity severity, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    char *formatted = awesomef(message, args);
    va_end(args);
    log(severity, _globalChannel.get(), formatted);
    free(formatted);
}

LogPipe &LogServer::log(Severity severity)
{
    return log(severity, _globalChannel.get());
}

/* free functions */

std::ostream &submit(std::ostream &os) {
    LogPipe *ls = static_cast<LogPipe*>(&os);
    ls->submit();
    return os;
}

const char *SeverityName(Severity severity)
{
    static const char* names[6] = {
        "debug",
        "info",
        "hint",
        "warn",
        "error",
        "panic"
    };

    uint64_t bitmask = 1;
    for (unsigned int i = 0; i < sizeof(names)/sizeof(const char *); i++)
    {
        if ((severity & bitmask) != 0) {
            return names[i];
        }
        bitmask <<= 1;
    }
    return 0;
}

const char *SeverityANSI(Severity severity)
{
    static const char* names[6] = {
        "\033[38;5;240m",
        "\033[38;5;33m",
        "\033[38;5;55m",
        "\033[38;5;214m",
        "\033[38;5;202m",
        "\033[1;38;5;196m"
    };

    uint64_t bitmask = 1;
    for (unsigned int i = 0; i < sizeof(names)/sizeof(const char *); i++)
    {
        if ((severity & bitmask) != 0) {
            return names[i];
        }
        bitmask <<= 1;
    }
    return 0;
}

LogServer *log = new LogServer();
LogServerHandle logHandle(log);

}
