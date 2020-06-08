#pragma once

#include "../texturedMesh.h"

class Box : public TexturedMesh
{
public:
    Box(float size, const char* diffusePath, const char* specularPath);
};
