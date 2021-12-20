#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl3.h>
#endif
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