#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <vector>
#include <glm/glm.hpp>
#include <optional>

#include "types.h"
#include "texture.h"

namespace mesh
{
    class Mesh
    {
        public:
            Mesh();
            Mesh(
                std::vector<glm::vec3> vertices,
                std::vector<glm::vec3> normals,
                std::vector<glm::vec2> uvs,
                std::vector<unsigned int> indices
            );
            ~Mesh();

            void load();
            void render();

            size_t nbVertices();

            void setDiffuseTexture(texture::Texture diffuse);
            void setSpecularTexture(texture::Texture specular);
            void setNormalTexture(texture::Texture normal);
        protected:
            std::vector<glm::vec3> vertices_;
            std::vector<glm::vec3> normals_;
            std::vector<glm::vec2> uvs_;
            std::vector<unsigned int> indices_;

            GLuint vaoID_;
            GLuint vboID_;
            GLuint eboID_;

            std::optional<texture::Texture> diffuseTexture_;
            std::optional<texture::Texture> specularTexture_;
            std::optional<texture::Texture> normalTexture_;
    };
}