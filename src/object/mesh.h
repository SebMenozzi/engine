#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <vector>
#include <glm/glm.hpp>

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

            void setDiffuseTexture(texture::Texture* texture);
            void setSpecularTexture(texture::Texture* texture);
            void setNormalTexture(texture::Texture* texture);

            void setMaterial(material::Material* material);
        protected:
            Buffer vertices_;
            Buffer normals_;
            Buffer uvs_;
            Buffer indices_;

            GLuint vaoID_;
            GLuint vboID_;
            GLuint eboID_;

            texture::Texture* diffuseTexture_ = nullptr;
            texture::Texture* specularTexture_ = nullptr;
            texture::Texture* normalTexture_ = nullptr;

            material::Material* material_ = nullptr;
    };
}