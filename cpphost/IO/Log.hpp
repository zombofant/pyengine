#ifndef _PYUNI_IO_LOG_H
#define _PYUNI_IO_LOG_H

#include <list>
#include <unordered_map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/functional/hash.hpp>

#include "Time.hpp"
#include "Stream.hpp"

namespace std {
template<>
struct hash<const std::string>
{
    size_t operator() (const std::string &str) const
    {
        return (size_t)boost::hash<std::string>()(str);
    }
};
}

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

class LogServer;

class LogBase {
    public:
        virtual void log(Severity severity, const char *message) = 0;
        virtual void logf(Severity severity, const char *message, ...) = 0;
};

class LogChannel: public LogBase {
    public:
        LogChannel(LogServer *server, const std::string name);
    private:
        LogServer *_server;
        const std::string _name;
    public:
        inline const char *getName() { return _name.c_str(); };
        virtual void log(Severity severity, const char *message);
        virtual void logf(Severity severity, const char *message, ...);
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

typedef boost::shared_ptr<LogChannel> LogChannelHandle;
typedef std::unordered_map<const std::string, LogChannelHandle> LogChannelMap;

class LogServer: public LogBase {
    public:
        LogServer();
    private:
        TimeStamp _startupTime;
        LogSinks _sinks;
        LogChannelMap _channels;
        LogChannelHandle _globalChannel;
    protected:
        void log(Severity severity, LogChannel *channel, const char *message);
        void logf(Severity severity, LogChannel *channel, const char *message, ...);
    public:
        void addSink(LogSinkHandle sink);
        LogChannelHandle getChannel(const std::string &name);
        void removeSink(LogSinkHandle sink);
    public:
        virtual void log(Severity severity, const char *message);
        virtual void logf(Severity severity, const char *message, ...);
    friend class LogChannel;
};

typedef boost::shared_ptr<LogServer> LogServerHandle;

const char *SeverityName(Severity severity);

extern LogServer *log;
extern LogServerHandle logHandle;

}

#endif
