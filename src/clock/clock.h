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
    /**
     * @brief Define the Clock object
     * @param void
     * @return void
    */
    Clock();

    /**
     * @brief Adjust the Clock according a time
     * @param time
     * @return void
    */
    void adjust(sint32 time);

    /**
     * @brief Retrieve the Clock time
     * @param void
     * @return time
    */
    sint32 getTime(void);

    /**
     * @brief Get number of milliseconds of the Clock
     * @param void
     * @return milliseconds
    */
    sint32 getTicks(void);
private:
    sint32 startTime;
};
