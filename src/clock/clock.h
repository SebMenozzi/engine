#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

#include "../types.h"

class Clock
{
public:
    Clock();
    void adjust(sint32 time);
    sint32 getTime(void);
    sint32 getTicks(void);
private:
    sint32 startTime;
};
