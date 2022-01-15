#pragma once

#include "heightmap.h"
#include "types.h"

namespace object
{
    class Ocean : public Heightmap
    {
        public:
            Ocean(
                float size,
                float scale
            );
            void updateHeights(uint32 time);
        private:
            float scale_;
    };
}