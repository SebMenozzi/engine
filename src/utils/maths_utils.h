#pragma once

#define _USE_MATH_DEFINES // for C++
#include <cmath>

namespace utils {
    const float PI = 3.14159265359;
    
    inline float clamp(float min, float max, float value) {
        return std::fmax(min, std::fmin(max, value));
    }

    inline double degrees_to_radians(double degrees) {
        return degrees * M_PI / 180.0;
    }

    inline double radians_to_degrees(double radians) {
        return radians * 180.0 / M_PI;
    }

    inline double random_double() {
        return rand() / (RAND_MAX + 1.0);
    }

    inline double random_double(double min, double max) {
        return min + (max - min) * random_double();
    }
}