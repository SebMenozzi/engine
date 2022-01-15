#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <iostream>
#include <string>

#include "types.h"

namespace texture
{
    class Texture
    {
        public:
            Texture(const char* filepath);
            Texture(uint8* data, int width, int height);
            ~Texture();
            bool load();
            GLuint getID() const;

        private:
            GLuint id_;
            const char* filepath_ = nullptr;

            uint8* data_ = nullptr;
            int width_ = -1;
            int height_ = -1;
    };
}