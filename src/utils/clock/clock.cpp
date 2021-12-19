#include "clock.h"

namespace utils
{
    Clock::Clock()
    {
        reset();
    }

    void Clock::reset()
    {
        start_time_ = getTicks();
    }

    time_t Clock::getTime()
    {
        return getTicks() - start_time_;
    }

    time_t Clock::getTicks()
    {
        struct timespec tp;
        clock_gettime(CLOCK_MONOTONIC, &tp);

        time_t seconds_to_milliseconds = tp.tv_sec * 1000;
        long nanoseconds_to_milliseconds = tp.tv_nsec / 1000000;

        return seconds_to_milliseconds + nanoseconds_to_milliseconds;
    }
}
