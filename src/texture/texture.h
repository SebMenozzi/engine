#pragma once

#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL3/gl.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "../utils/imageUtils.h"

class Texture
{
    public:
        Texture(const char* imageFilePath);
        ~Texture();
        bool load();
        GLuint getID() const;

    private:
        GLuint id;
        const char* imageFilePath;
};
