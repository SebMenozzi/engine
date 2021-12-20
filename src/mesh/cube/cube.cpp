#include "cube.h"

namespace mesh
{
    Cube::Cube(float size): Mesh()
    {
        size /= 2;

        vertices_ = {
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

        normals_ = {
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

        uvs_ = {
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
    }
}