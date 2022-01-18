#pragma once

#include <algorithm>

#include "heightmap.h"
#include "types.h"

namespace object
{
    class Terrain : public Heightmap
    {
        public:
            Terrain(const char* filepath, float scale, float minHeight, float maxHeight);
        private:
            float minHeight_;
            float maxHeight_;

            void addVertices_(std::vector<glm::vec3>* vertices, float x, float z);
            glm::vec3 computeNormal_(float x, float z);
    };
}