#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>

namespace texture
{
    class CubemapTexture
    {
        public:
            CubemapTexture(std::vector<std::string> faces);
            ~CubemapTexture();

            bool load();
            GLuint getID() const;

        private:
            GLuint id_;
            std::vector<std::string> faces_;
    };
}