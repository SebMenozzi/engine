#pragma once

#include "../mesh.h"

class Plane : public Mesh
{
public:
    Plane(float size, const char* vertexPath, const char* fragmentPath);
};
