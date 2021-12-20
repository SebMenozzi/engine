#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <vector>

#include "types.h"
#include "texture.h"

namespace mesh
{
    class Mesh
    {
        public:
            Mesh(
                const char* diffusePath, 
                const char* specularPath,
                const char* normalPath
            );
            ~Mesh();

            void load();
            void render();

        protected:
            std::vector<float> vertices_;
            std::vector<unsigned int> indices_;
            std::vector<float> normals_;
            std::vector<float> uvs_;

            GLuint vaoID_;
            GLuint vboID_;
            GLuint eboID_;

            texture::Texture diffuse_;
            texture::Texture specular_;
            texture::Texture normal_;
    };
}