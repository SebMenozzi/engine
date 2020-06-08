#pragma once

#include <vector>

#include "../cube/cube.h"
#include "../../cubemap/cubemap.h"

class Skybox : public Cube
{
public:
    Skybox(float size, std::vector<std::string> faces);
    ~Skybox() = default;

    void load();
    void render();

private:
    Cubemap cubemap;
    std::vector<std::string> faces;
};
