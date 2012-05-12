#ifndef _PYUNI_IO_LOG_H
#define _PYUNI_IO_LOG_H

#include <list>

#include <boost/shared_ptr.hpp>

#include "Time.hpp"
#include "Stream.hpp"

namespace PyUni {

enum Severity {
    Debug           = 0x01,
    Information     = 0x02,
    Hint            = 0x04,
    Warning         = 0x08,
    Error           = 0x10,
    Panic           = 0x20
};

typedef uint64 SeverityMask;

const SeverityMask All = 0x3F;

struct LogChannel {
    LogChannel(const std::string aName);
    
    const std::string name;
};

class LogSink {
    public:
        LogSink(uint64_t mask);
        virtual ~LogSink() {};
    private:
        uint64_t _mask;
    protected:
        virtual void doLog(TimeFloat timestamp, Severity severity,
            LogChannel *channel, const char *message) = 0;
    public:
        void log(TimeFloat timestamp, Severity severity,
            LogChannel *channel, const char *message);
};

typedef boost::shared_ptr<LogSink> LogSinkHandle;
typedef std::list<LogSinkHandle> LogSinks;

class LogStreamSink: public LogSink {
    public:
        LogStreamSink(uint64_t mask, StreamHandle stream);
    private:
        StreamHandle _streamHandle;
        Stream *_stream;
    public:
        virtual void doLog(TimeFloat timestamp, Severity severity,
            LogChannel *channel, const char *message);
};

class LogXMLSink: public LogSink {
    public:
        LogXMLSink(uint64_t mask, StreamHandle stream, const std::string xsltFile);
        virtual ~LogXMLSink();
    private:
        StreamHandle _streamHandle;
        Stream *_stream;
        const std::string _xsltFile;
    public:
        virtual void doLog(TimeFloat timestamp, Severity severity,
            LogChannel *channel, const char *message);
};

class LogServer {
    public:
        LogServer();
    private:
        TimeStamp _startupTime;
        LogSinks _sinks;
    public:
        void addSink(LogSinkHandle sink);
        void removeSink(LogSinkHandle sink);
    public:
        void log(Severity severity, const char *message);
        void log(Severity severity, LogChannel *channel, const char *message);
        void logf(Severity severity, const char *message, ...);
        void logf(Severity severity, LogChannel *channel, const char *message, ...);
};

const char *SeverityName(Severity severity);

extern LogServer *log;

}

#endif
