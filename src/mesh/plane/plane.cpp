#include "plane.h"

namespace mesh
{
    Plane::Plane(float size): Mesh()
    {
        size /= 2;

        vertices_ = {
            glm::vec3(-size, -0.5f, -size),
            glm::vec3(-size, -0.5f, size),
            glm::vec3(size, -0.5f, size),

            glm::vec3(size, -0.5f,  size),
            glm::vec3(size, -0.5f, -size),
            glm::vec3(-size, -0.5f, -size)
        };

        normals_ = {
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),

            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        };

        uvs_ = {
            glm::vec2(0.0f,  0.0f),
            glm::vec2(0.0f,  size),
            glm::vec2(size, size),

            glm::vec2(size,  size),
            glm::vec2(size, 0.0f),
            glm::vec2(0.0f, 0.0f)
        };
    }
}