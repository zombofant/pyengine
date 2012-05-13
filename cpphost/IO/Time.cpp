#include "Time.hpp"

namespace PyUni {

/* free functions */

timespec nanotime() {
    timespec result;
    clock_gettime(CLOCK_MONOTONIC_RAW, &result);
    return result;
}

TimeFloat timeToDouble(const timespec &time) {
    return TimeFloat(time.tv_sec) + TimeFloat(time.tv_nsec) / 1000000000.;
}

TimeFloat timeIntervalToDouble(const timespec &prev, const timespec &curr) {
    TimeFloat result = (curr.tv_sec - prev.tv_sec);
    result += TimeFloat(curr.tv_nsec - prev.tv_nsec) / (1000000000.);
    return result;
}

}
