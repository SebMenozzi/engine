#pragma once

#include <vector>

#include "mesh.h"
#include "maths_utils.h"
#include "types.h"

namespace object
{
    class Heightmap : public Mesh
    {
        public:
            Heightmap(uint32 size);
            float getHeight(float x, float z);
        protected:
            uint32 size_;
            std::vector<std::vector<float> > heights_;

            float getTileHeight_(int tileX, int tileZ);
    };
}