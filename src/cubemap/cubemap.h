#pragma once

#include <vector>

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

class Cubemap
{
public:
    Cubemap(std::vector<std::string> faces);
    ~Cubemap();
    bool load();
    GLuint getID() const;

private:
    GLuint id;
    std::vector<std::string> faces;
};
