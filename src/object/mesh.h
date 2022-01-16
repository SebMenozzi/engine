#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <vector>
#include <glm.hpp>

#include "types.h"
#include "texture.h"
#include "material.h"
#include "object.h"

namespace object
{
    class Mesh : public Object
    {
        public:
            Mesh();
            Mesh(
                Buffer vertices,
                Buffer normals,
                Buffer uvs,
                Buffer indices
            );
            ~Mesh() override;

            void load() override;
            void render() override;
            void useShader(shader::Shader* shader) override;

            size_t nbVertices();
            size_t nbIndices();

            void addTexture(texture::Texture* texture);

            void setMaterial(material::Material* material);
        protected:
            Buffer vertices_;
            Buffer normals_;
            Buffer uvs_;
            Buffer indices_;

            GLuint vaoID_;
            GLuint vboID_;
            GLuint eboID_;

            std::vector<texture::Texture*> textures_;

            material::Material* material_ = nullptr;
    };
}