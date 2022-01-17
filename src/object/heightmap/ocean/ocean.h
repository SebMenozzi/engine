#pragma once

#include "heightmap.h"
#include "types.h"

#include <array>

namespace object
{
    class Ocean : public Heightmap
    {
        public:
            Ocean(uint32 size, float scale);
            
            void updateHeights(uint32 time);
    private:
            constexpr static int perlinLoopDuration_ = 10; // in seconds
            constexpr static int perlinLoopFps_ = 2;
            constexpr static float perlinLoopPeriod = 1.f / perlinLoopFps_;
            constexpr static int perlinLoopFrameLength_ = perlinLoopDuration_ * perlinLoopFps_;

            std::vector<std::vector<std::array<float, perlinLoopFrameLength_>>> perlinLoopFrames_;

            float computePerlinHeight_(int x, int z, float time);
            float interpolatePerlinHeight_(int x, int z, uint32 time);
    };
}