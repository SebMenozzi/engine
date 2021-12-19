#pragma once

#include <time.h>

#include "types.h"

namespace utils
{
    class Clock
    {
        public:
            Clock();
            void reset();
            time_t getTime();
            time_t getTicks(void);
        private:
            time_t start_time_;
    };
}