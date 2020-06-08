#pragma once

#include "../texturedMesh.h"

class Floor : public TexturedMesh
{
public:
    Floor(float size, const char* diffusePath, const char* specularPath);
};
