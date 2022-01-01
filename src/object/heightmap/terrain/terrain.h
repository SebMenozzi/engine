#pragma once

#include <SDL2/SDL_image.h>

#include "heightmap.h"
#include "image_utils.h"

namespace object
{
    class Terrain : public Heightmap
    {
        public:
            Terrain(const char* filepath, float scale, float minHeight, float maxHeight);
        private:
            float scale_;
            float minHeight_;
            float maxHeight_;
            SDL_Surface* image_;
            
            void addVertices_(std::vector<glm::vec3>* vertices, float x, float z);
            glm::vec3 computeNormal_(float x, float z);
    };
}