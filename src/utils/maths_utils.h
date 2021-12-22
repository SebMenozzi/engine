#pragma once

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <glm/glm.hpp>

namespace utils
{
    const float PI = 3.14159265359;

    inline float clamp(float value, float min, float max)
    {
        return std::fmax(min, std::fmin(max, value));
    }

    inline float lerp(float t, float a, float b)
    {
        return a + t * (b - a);
    }

    inline float getPercentageAlong(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;

        return glm::dot(ac, glm::normalize(ab)) / glm::length(ab);
    }

    inline float scale(float value, float oldMin, float oldMax, float newMin, float newMax)
    {
        return ((newMax - newMin) * (value - oldMin) / (oldMax - oldMin)) + newMin;
    }

    inline double degrees_to_radians(double degrees)
    {
        return degrees * M_PI / 180.0;
    }

    inline double radians_to_degrees(double radians)
    {
        return radians * 180.0 / M_PI;
    }

    inline double random_double()
    {
        return rand() / (RAND_MAX + 1.0);
    }

    inline double random_double(double min, double max)
    {
        return min + (max - min) * random_double();
    }
}