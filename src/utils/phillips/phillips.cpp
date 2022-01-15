#include "phillips.h"

namespace utils
{
    // Useful Links:
    // - http://evasion.imag.fr/~Fabrice.Neyret/images/fluids-nuages/waves/Jonathan/articlesCG/waterslides2001.pdf

    Phillips::Phillips(float size):
        size_(size),
        gravity_(9.81),
        windDirection_(1.0, 1.0),
        windSpeed_(50.0),
        waveAmplitude_(0.0002f)
    {
        computeH0AndH1_();
    }

    float Phillips::randomGaussian_()
    {
        std::mt19937 generator;
        float mean = 0.0;
        float stddev  = 1.0;
        std::normal_distribution<> normal{mean, stddev};

        return normal(generator);
    }

    float Phillips::spectrum_(glm::vec2 k)
    {
        float kLength = glm::length(k);

        if (kLength < 1e-6f)
            return 0;
        
        float kDotV = glm::dot(
            glm::normalize(k), 
            glm::normalize(windDirection_)
        );

        float L = windSpeed_ * windSpeed_ / gravity_;

        return (
            waveAmplitude_ *
            exp(-1.0 / (kLength * kLength * L * L)) /
            (kLength * kLength * kLength * kLength) *
            (kDotV * kDotV * kDotV * kDotV)
        );
    }

    void Phillips::computeH0AndH1_()
    {
        std::vector<complex> h0tmp;
        std::vector<complex> h1tmp;

        for (size_t x = 0; x < size_; ++x)
        {
            h0tmp.clear();
            h1tmp.clear();

            for (size_t z = 0; z < size_; ++z)
            {
                glm::vec2 k = glm::vec2(
                    (2 * M_PI * (x - size_ / 2)) / 5000,
                    (2 * M_PI * (z - size_ / 2)) / 5000
                );

                float er = randomGaussian_();
                float ei = randomGaussian_();

                float sqrt2 = 1 / sqrt(2);
                float spec = spectrum_(k);
                float sqrtspec = sqrt(spec);

                complex h0 = std::complex(sqrt2 * er * sqrtspec, sqrt2 * ei * sqrtspec);
                complex h1 = std::conj(h0);

                h0tmp.push_back(h0);
                h1tmp.push_back(h1);
            }

            h0_.push_back(h0tmp);
            h1_.push_back(h1tmp);
        }
    }
}