#include "Log.hpp"

#include <iostream>
#include <cstdio>
#include <cstdarg>

#include "Misc/Int.hpp"

namespace PyUni {

const sizeint AWESOME_BUFFER_SIZE = 512;

char *awesomef(const char *message, va_list args, sizeuint *length = 0)
{
    char *buffer = (char*)malloc(AWESOME_BUFFER_SIZE);
    sizeint actualLength = vsnprintf(buffer, AWESOME_BUFFER_SIZE, message, args);
    if (actualLength >= AWESOME_BUFFER_SIZE) {
        buffer = (char*)realloc(buffer, actualLength+1);
        assert(vsnprintf(buffer, actualLength+1, message, args) == actualLength);
    } else {
        buffer = (char*)realloc(buffer, actualLength);
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

/* PyUni::IO::LogChannel */

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

/* PyUni::IO::LogSink */

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

/* PyUni::IO::LogStreamSink */

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
}

/* PyUni::IO::LogXMLSink */

LogXMLSink::LogXMLSink(uint64_t mask, StreamHandle stream, const std::string xsltFile):
    LogSink::LogSink(mask),
    _streamHandle(stream),
    _stream(stream.get())
{
    _stream->writeNullTerminated("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<?xml-stylesheet type=\"text/xsl\" href=\"");
    _stream->writeNullTerminated(xsltFile.c_str());
    _stream->writeNullTerminated("\"?>\n\
<log>\n");
}

LogXMLSink::~LogXMLSink()
{
    _stream->writeNullTerminated("</log>");
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
    free(formatted);
    }

/* PyUni::IO::LogServer */

LogServer::LogServer():
    _startupTime(nanotime()),
    _sinks(),
    _channels(),
    _globalChannel(getChannel("global"))
{
    
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

void LogServer::log(Severity severity, LogChannel *channel, const char *message)
{
    TimeFloat timestamp = timeIntervalToDouble(_startupTime, nanotime());
    for (auto it = _sinks.begin(); it != _sinks.end(); it++)
    {
        LogSink *sink = (*it).get();
        sink->log(timestamp, severity, channel, message);
    }
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

void LogServer::logf(Severity severity, LogChannel *channel, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    char *formatted = awesomef(message, args);
    va_end(args);
    log(severity, channel, formatted);
    free(formatted);
}

/* free functions */

const char *SeverityName(Severity severity)
{
    static const char* names[6] = {
        "debug",
        "info",
        "hint",
        "warn",
        "error"
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

LogServer *log = new LogServer();
LogServerHandle logHandle(log);

}
