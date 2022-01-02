#pragma once

#include <iostream>
#include <vector>
#include <glm.hpp>
#include <random>

#include "types.h"
#include "image_utils.h"

namespace utils
{
    class Phillips
    {
        public:
            Phillips(float size);
        private:
            float size_;
            float gravity_;
            glm::vec2 windDirection_;
            float windSpeed_;
            float waveAmplitude_;
            std::vector<std::vector<complex> > h0_;
            std::vector<std::vector<complex> > h1_;

            float randomGaussian_();
            float spectrum_(glm::vec2 k);
            void computeH0AndH1_();
    };
}