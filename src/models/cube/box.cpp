#include "box.h"

Box::Box(float size, const char* vertexPath, const char* fragmentPath, const char* diffusePath, const char* specularPath)
: TexturedMesh(size, vertexPath, fragmentPath, diffusePath, specularPath)
{
    size /= 2;

    this->vertices = {
        -size, -size, -size,
        size, -size, -size,
        size, size, -size,
        size, size, -size,
        -size, size, -size,
        -size, -size, -size,

        -size, -size, size,
        size, -size, size,
        size, size, size,
        size, size, size,
        -size, size, size,
        -size, -size, size,

        -size, size, size,
        -size, size, -size,
        -size, -size, -size,
        -size, -size, -size,
        -size, -size, size,
        -size, size, size,

        size, size, size,
        size, size, -size,
        size, -size, -size,
        size, -size, -size,
        size, -size, size,
        size, size, size,

        -size, -size, -size,
        size, -size, -size,
        size, -size, size,
        size, -size, size,
        -size, -size, size,
        -size, -size, -size,

        -size, size, -size,
        size, size, -size,
        size, size, size,
        size, size, size,
        -size, size, size,
        -size, size, -size
    };

    this->normals = {
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

    this->textureCoord = {
        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,

        0.0f,  0.0f,
        1.0f,  0.0f,
        1.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,

        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,

        1.0f,  0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,

        0.0f,  1.0f,
        1.0f,  1.0f,
        1.0f,  0.0f,
        1.0f,  0.0f,
        0.0f,  0.0f,
        0.0f,  1.0f,

        0.0f,  1.0f,
        1.0f,  1.0f,
        1.0f,  0.0f,
        1.0f,  0.0f,
        0.0f,  0.0f,
        0.0f,  1.0f
    };

}
