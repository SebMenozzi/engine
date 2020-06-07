#include "box.h"

Box::Box(float size, const char* vertexPath, const char* fragmentPath, const char* diffusePath, const char* specularPath)
: TexturedMesh(size, vertexPath, fragmentPath, diffusePath, specularPath)
{
    size /= 2;

    this->vertices = {
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
