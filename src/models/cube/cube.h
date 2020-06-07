#pragma once

#include "../mesh.h"

class Cube : public Mesh
{
public:
    Cube(float size, const char* vertexPath, const char* fragmentPath);
};
