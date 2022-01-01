#pragma once

#include "heightmap.h"

namespace object
{
    class Ocean : public Heightmap
    {
        public:
            Ocean(
                float size,
                float height
            );
        private:
            float height_;
    };
}