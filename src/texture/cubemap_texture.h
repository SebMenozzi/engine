#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <iostream>
#include <vector>
#include <string>

#include "types.h"

namespace texture
{
    class CubemapTexture
    {
        public:
            CubemapTexture(std::vector<const char*> faces);
            ~CubemapTexture();

            bool load();
            GLuint getID() const;

        private:
            GLuint id_;
            std::vector<const char*> faces_;
    };
}