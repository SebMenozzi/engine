#pragma once

#include <vector>

#include "./mesh.h"
#include "../texture/texture.h"
#include "../types.h"
#include "../camera/camera.h"

class TexturedMesh : public Mesh
{
public:
    TexturedMesh(const char* diffusePath, const char* specularPath);
    void load();
    void render();

protected:
    Texture diffuse;
    Texture specular;

    std::vector<float> textureCoord;
};
