#pragma once

#include "heightmap.h"

#include <array>

namespace object
{
    class Ocean : public Heightmap
    {
        public:
            Ocean(
                float size,
                float height
            );

            void updateHeights(float t);
            void updateVerticesAndNormals();

    private:
        std::vector<glm::vec3> glmVertices_, glmNormals_;
        float height_;
    };
}