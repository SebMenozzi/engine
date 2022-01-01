#include "cube.h"

namespace object
{
    Cube::Cube(float size): Mesh()
    {
        size /= 2;

        auto vertices = new std::vector<glm::vec3> {
            // Back face
            glm::vec3(-size, -size, -size),
            glm::vec3(-size, size, -size),
            glm::vec3(size, size, -size),
            glm::vec3(size, size, -size),
            glm::vec3(size, -size, -size),
            glm::vec3(-size, -size, -size),

            // Front face
            glm::vec3(size, size, size),
            glm::vec3(-size, size, size),
            glm::vec3(-size, -size, size),
            glm::vec3(-size, -size, size),
            glm::vec3(size, -size, size),
            glm::vec3(size, size, size),

            // Left face
            glm::vec3(-size, size, size),
            glm::vec3(-size, size, -size),
            glm::vec3(-size, -size, size),
            glm::vec3(-size, -size, size),
            glm::vec3(-size, size, -size),
            glm::vec3(-size, -size, -size),

            // Right face
            glm::vec3(size, size, size),
            glm::vec3(size, -size, size),
            glm::vec3(size, size, -size),
            glm::vec3(size, size, -size),
            glm::vec3(size, -size, size),
            glm::vec3(size, -size, -size),

            // Below face
            glm::vec3(size, -size, size),
            glm::vec3(-size, -size, size),
            glm::vec3(size, -size, -size),
            glm::vec3(size, -size, -size),
            glm::vec3(-size, -size, size),
            glm::vec3(-size, -size, -size),

            // Above face
            glm::vec3(size, size, size),
            glm::vec3(size, size, -size),
            glm::vec3(-size, size, size),
            glm::vec3(-size, size, size),
            glm::vec3(size, size, -size),
            glm::vec3(-size, size, -size)
        };

        auto normals = new std::vector<glm::vec3> {
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),

            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),

            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),

            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),

            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),

            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        };

        auto uvs = new std::vector<glm::vec2> {
            // Back face
            glm::vec2(0.0f,  0.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(1.0f,  1.0f),
            glm::vec2(1.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f,  0.0f),

            // Front face
            glm::vec2(1.0f,  1.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(0.0f,  0.0f),
            glm::vec2(0.0f,  0.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(1.0f,  1.0f),

            // Left face
            glm::vec2(0.0f,  1.0f),
            glm::vec2(1.0f,  1.0f),
            glm::vec2(0.0f,  0.0f),
            glm::vec2(0.0f,  0.0f),
            glm::vec2(1.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),

            // Right face
            glm::vec2(1.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f,  0.0f),

            // Below face
            glm::vec2(1.0f,  1.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(0.0f,  0.0f),

            // Above face
            glm::vec2(1.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(0.0f,  1.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(0.0f,  0.0f)
        };

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };
        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };
        uvs_ = Buffer { reinterpret_cast<const uint8*>(uvs->data()), sizeof(glm::vec2), uvs->size() };
    }
}