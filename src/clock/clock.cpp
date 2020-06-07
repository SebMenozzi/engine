#include "clock.h"

Clock::Clock()
{
    this->startTime = this->getTicks();
}

void Clock::adjust(sint32 time)
{
    this->startTime = this->getTicks() - time;
}

sint32 Clock::getTime(void)
{
    return this->getTicks() - this->startTime;
}

sint32 Clock::getTicks(void)
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000);
}
