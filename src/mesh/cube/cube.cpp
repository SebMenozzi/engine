#include "cube.h"

namespace mesh
{
    Cube::Cube(
        float size, 
        const char* diffusePath, 
        const char* specularPath,
        const char* normalPath
    ):
        Mesh(diffusePath, specularPath, normalPath)
    {
        size /= 2;

        vertices_ = {
            // Back face
            -size, -size, -size,
            -size, size, -size,
            size, size, -size,
            size, size, -size,
            size, -size, -size,
            -size, -size, -size,

            // Front face
            size, size, size,
            -size, size, size,
            -size, -size, size,
            -size, -size, size,
            size, -size, size,
            size, size, size,

            // Left face
            -size, size, size,
            -size, size, -size,
            -size, -size, size,
            -size, -size, size,
            -size, size, -size,
            -size, -size, -size,

            // Right face
            size, size, size,
            size, -size, size,
            size, size, -size,
            size, size, -size,
            size, -size, size,
            size, -size, -size,

            // Below face
            size, -size, size,
            -size, -size, size,
            size, -size, -size,
            size, -size, -size,
            -size, -size, size,
            -size, -size, -size,

            // Above face
            size, size, size,
            size, size, -size,
            -size, size, size,
            -size, size, size,
            size, size, -size,
            -size, size, -size
        };

        normals_ = {
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        };

        uvs_ = {
            // Back face
            0.0f,  0.0f,
            0.0f,  1.0f,
            1.0f,  1.0f,
            1.0f,  1.0f,
            1.0f,  0.0f,
            0.0f,  0.0f,

            // Front face
            1.0f,  1.0f,
            0.0f,  1.0f,
            0.0f,  0.0f,
            0.0f,  0.0f,
            1.0f,  0.0f,
            1.0f,  1.0f,

            // Left face
            0.0f,  1.0f,
            1.0f,  1.0f,
            0.0f,  0.0f,
            0.0f,  0.0f,
            1.0f,  1.0f,
            1.0f,  0.0f,

            // Right face
            1.0f,  1.0f,
            1.0f,  0.0f,
            0.0f,  1.0f,
            0.0f,  1.0f,
            1.0f,  0.0f,
            0.0f,  0.0f,

            // Below face
            1.0f,  1.0f,
            0.0f,  1.0f,
            1.0f,  0.0f,
            1.0f,  0.0f,
            0.0f,  1.0f,
            0.0f,  0.0f,

            // Above face
            1.0f,  1.0f,
            1.0f,  0.0f,
            0.0f,  1.0f,
            0.0f,  1.0f,
            1.0f,  0.0f,
            0.0f,  0.0f
        };
    }
}