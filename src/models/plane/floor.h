#pragma once

#include "../texturedMesh.h"

class Floor : public TexturedMesh
{
public:
    Floor(float size, const char* vertexPath, const char* fragmentPath, const char* diffusePath, const char* specularPath);
};
