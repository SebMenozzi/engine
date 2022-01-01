#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "image_utils.h"

namespace texture
{
    class Texture
    {
        public:
            Texture(const char* filePath);
            Texture(SDL_Surface* image);
            ~Texture();
            bool load();
            GLuint getID() const;

        private:
            GLuint id_;
            const char* filePath_ = nullptr;
            SDL_Surface* image_ = nullptr;
    };
}