#include "floor.h"

Floor::Floor(float size, const char* vertexPath, const char* fragmentPath, const char* diffusePath, const char* specularPath)
: TexturedMesh(size, vertexPath, fragmentPath, diffusePath, specularPath)
{
    this->vertices = {
        size, -0.5f, size,
        -size, -0.5f, size,
        -size, -0.5f, -size,

        size, -0.5f,  size,
        -size, -0.5f, -size,
        size, -0.5f, -size
    };

    this->normals = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    this->textureCoord = {
        size,  0.0f,
        0.0f,  0.0f,
        0.0f, size,

        size,  0.0f,
        0.0f, size,
        size, size
    };
}
