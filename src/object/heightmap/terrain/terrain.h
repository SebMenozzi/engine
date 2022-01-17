#pragma once

#include <algorithm>

#include "heightmap.h"
#include "types.h"

namespace object
{
    class Terrain : public Heightmap
    {
        public:
            Terrain(uint32 size, float scale);
    };
}