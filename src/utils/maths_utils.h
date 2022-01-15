#pragma once

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <glm.hpp>

namespace utils
{
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
        return (
            (newMax - newMin) *
            ((value - oldMin) / (oldMax - oldMin)) + 
            newMin
        );
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