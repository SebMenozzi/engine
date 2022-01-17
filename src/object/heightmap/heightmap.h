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
            Heightmap(uint32 size, float scale);
            float getHeight(float x, float z);
        protected:
            uint32 size_;
            float scale_;
            std::vector<std::vector<float> > heights_;

            float getTileHeight_(int tileX, int tileZ);
            void computeNormals_(float strength);
            glm::vec3 computeNormal_(int x, int z, float strength);
    };
}