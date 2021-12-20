#pragma once

#include <GL/glew.h>
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
            ~Texture();
            bool load();
            GLuint getID() const;

        private:
            GLuint id_;
            const char* filePath_;
    };
}