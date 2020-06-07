#include "plane.h"

Plane::Plane(float size, const char* vertexPath, const char* fragmentPath): Mesh(size, vertexPath, fragmentPath)
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

    this->colors = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

}
