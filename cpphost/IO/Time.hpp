#ifndef _PYUNI_IO_TIME_H
#define _PYUNI_IO_TIME_H

#include <ctime>

namespace PyUni {

typedef double TimeFloat;
typedef timespec TimeStamp;

/* free functions */

TimeStamp nanotime();
TimeFloat timeToDouble(const TimeStamp &time);
TimeFloat timeIntervalToDouble(const TimeStamp &prev, const TimeStamp &curr);

}

#endif
