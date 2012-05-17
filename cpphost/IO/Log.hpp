/**********************************************************************
File name: Log.hpp
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
#ifndef _PYUNI_IO_LOG_H
#define _PYUNI_IO_LOG_H

#include <list>
#include <unordered_map>
#include <string>
#include <sstream>

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
class LogChannel;

class LogPipe: public std::ostringstream {
    public:
        LogPipe(LogServer *server, Severity severity,
            LogChannel *channel);
        virtual ~LogPipe();
    private:
        LogServer *_server;
        Severity _severity;
        LogChannel *_channel;
    public:
        void submit();
    friend class LogServer;
};

class LogBase {
    public:
        virtual ~LogBase() {};
    public:
        virtual void log(Severity severity, const char *message) = 0;
        virtual void logf(Severity severity, const char *message, ...) = 0;
        virtual LogPipe &log(Severity severity) = 0;
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
        virtual LogPipe &log(Severity severit);
};

/**
 * Abstract base class for a log sink.
 *
 * You must not (and are unable to) instanciate this class directly. But
 * you will want to use it as a base class for your own logging sinks.
 *
 * For this, you have to override the doLog protected method to do the
 * logging. You do not need to check for the log mask to match the
 * severity, this is done on a higher level for you.
 */
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

/**
 * Log sink which simply writes the log entries as plain text to the
 * given stream, separated by newlines.
 *
 * Each line has the following format:
 * [%12.4f] [%s] [%s] %s\n
 * with the arguments: timestamp, severity, channel, message
 */
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

/**
 * Log sink which creates an xml file in the given stream, referring
 * to the given xslt file.
 *
 * Each log message is logged in the following format:
 * <message>
 *   <timestamp>%f</timestamp>
 *   <severity>%s</severity>
 *   <channel>%s</channel>
 *   <text>%s</text>
 * </message>
 *
 * The root node is <log />
 */
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

/**
 * Log service which allows broadcasting to several different LogSinks
 * using a standardized simple interface.
 *
 * The log server supports multiple useful interfaces for logging: A
 * simple interface taking just a null terminated utf-8 string, a
 * sprintf like interface and an interface compatible to stdc++ stream
 * operators.
 *
 * Additionally, one can define multiple log channels whose name will be
 * added to the log entry for simple grepping. All LogSinks support
 * masks for which severities will be logged to the specific sink.
 */
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
        LogPipe &log(Severity severity, LogChannel *channel);
        void log(LogPipe *stream);
    public:
        /**
         * Attach a sink to the log server.
         *
         * Only attached sinks will recieve messages sent to the log
         * server.
         *
         * @param sink Sink to attach.
         */
        void addSink(LogSinkHandle sink);

        /**
         * Request and return a channel.
         *
         * Requests a channel with the given name and returns it. If the
         * channel exists already, no new instance is created.
         *
         * @param name Channel name
         */
        LogChannelHandle getChannel(const std::string &name);

        /**
         * Remove the given sink from the server.
         *
         * The sink will not receive any further messages from this
         * server.
         *
         * @param sink Sink to remove.
         */
        void removeSink(LogSinkHandle sink);
    public:
        /**
         * Log a preformatted message.
         *
         * The message is broadcast to all currently attached log sinks.
         * Log sinks which are attached later will not recieve this
         * message.
         *
         * @param severity Severity of the message
         * @param message Message contents as null terminated utf8
         *                string.
         */
        virtual void log(Severity severity, const char *message);

        /**
         * Log a message using sprintf syntax.
         *
         * This works the same as log, but you can use sprintf syntax
         * to format the message.
         *
         * @param severity Severity of the message
         * @param message Message format string
         * @param ... Format arguments.
         */
        virtual void logf(Severity severity, const char *message, ...);

        /**
         * Return a LogPipe with the given severity.
         *
         * This creates a LogPipe instance with the given severity which
         * can be used to use the stdc++ stream interface with
         * LogServer instances.
         *
         * You must pipe submit to the LogPipe to free its resources and
         * submit the log entry to the LogServer.
         *
         * @param severity Severity of the message.
         * @return New LogPipe instance reference.
         */
        virtual LogPipe &log(Severity severity);
    friend class LogChannel;
    friend class LogPipe;
};

typedef boost::shared_ptr<LogServer> LogServerHandle;

const char *SeverityName(Severity severity);

std::ostream &submit(std::ostream &os);

extern LogServer *log;
extern LogServerHandle logHandle;

}

#endif
