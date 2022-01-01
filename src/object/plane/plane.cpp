#include "plane.h"

namespace object
{
    Plane::Plane(float size): Mesh()
    {
        size /= 2;

        auto vertices = new std::vector<glm::vec3> {
            glm::vec3(-size, -0.5f, -size),
            glm::vec3(-size, -0.5f, size),
            glm::vec3(size, -0.5f, size),

            glm::vec3(size, -0.5f,  size),
            glm::vec3(size, -0.5f, -size),
            glm::vec3(-size, -0.5f, -size)
        };

        vertices_ = Buffer { reinterpret_cast<const uint8*>(vertices->data()), sizeof(glm::vec3), vertices->size() };

        auto normals = new std::vector<glm::vec3> {
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),

            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        };

        normals_ = Buffer { reinterpret_cast<const uint8*>(normals->data()), sizeof(glm::vec3), normals->size() };

        auto uvs = new std::vector<glm::vec2> {
            glm::vec2(0.0f,  0.0f),
            glm::vec2(0.0f,  size),
            glm::vec2(size, size),

            glm::vec2(size,  size),
            glm::vec2(size, 0.0f),
            glm::vec2(0.0f, 0.0f)
        };

        uvs_ = Buffer { reinterpret_cast<const uint8*>(uvs->data()), sizeof(glm::vec2), uvs->size() };
    }
}