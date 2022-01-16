#pragma once

#include "heightmap.h"
#include "types.h"

#include <array>

namespace object
{
    class Ocean : public Heightmap
    {
        public:
            Ocean(
                float size,
                float scale
            );

            float computePerlinHeight(int x, int z, float time);
            float interpolatePerlinHeight(int x, int z, uint32 time);
            void updateHeights(uint32 time);
            glm::vec3 computeNormal(int x, int z);
            void updateNormals();

    private:
            float scale_;
            constexpr static int perlinLoopDuration_ = 10; // in seconds
            constexpr static int perlinLoopFps_ = 2;
            constexpr static float perlinLoopPeriod = 1.f / perlinLoopFps_;
            constexpr static int perlinLoopFrameLength_ = perlinLoopDuration_ * perlinLoopFps_;

            std::vector<std::vector<std::array<float, perlinLoopFrameLength_>>> perlinLoopFrames_;
    };
}