#pragma once

#include <time.h>
#include <vector>
#include <SDL2/SDL_image.h>

#include "mesh.h"
#include "image_utils.h"
#include "maths_utils.h"
#include "types.h"

namespace mesh
{
    class Heightmap : public Mesh
    {
    public:
        Heightmap(const char* filepath, float scale, float minHeight, float maxHeight);
        float getHeight(float x, float z);
    private:
        float scale_;
        float minHeight_;
        float maxHeight_;

        std::vector<std::vector<float> > heights_;
        SDL_Surface* image_;

        float getTileHeight_(int tileX, int tileZ);
        void addVertices_(float x, float z);
        glm::vec3 computeNormal_(float x, float z);
    };
}