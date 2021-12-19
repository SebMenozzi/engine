#include "plane.h"

namespace mesh
{
    Plane::Plane(
        float size,
        const char* diffusePath, 
        const char* specularPath,
        const char* normalPath
    ):
        Mesh(diffusePath, specularPath, normalPath)
    {
        size /= 2;
        
        vertices_ = {
            -size, -0.5f, -size,
            -size, -0.5f, size,
            size, -0.5f, size,

            size, -0.5f,  size,
            size, -0.5f, -size,
            -size, -0.5f, -size
        };

        normals_ = {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        };

        uvs_ = {
            0.0f,  0.0f,
            0.0f,  size,
            size, size,

            size,  size,
            size, 0.0f,
            0.0f, 0.0f
        };
    }
}