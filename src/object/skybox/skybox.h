#pragma once

#include <vector>

#include "cube.h"
#include "cubemap_texture.h"

namespace object
{
    class Skybox: public Cube
    {
        public:
            Skybox(float size, std::vector<const char*> filepaths);
            ~Skybox() = default;

            void load();
            void render();

        private:
            texture::CubemapTexture cubemapTexture_;
            std::vector<const char*> faces_;
    };
}