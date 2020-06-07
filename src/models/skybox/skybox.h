#pragma once

#include <vector>

#include "../cube/cube.h"
#include "../../cubemap/cubemap.h"

class Skybox : public Cube
{
public:
    Skybox(float size, const char* vertexPath, const char* fragmentPath, std::vector<std::string> faces);
    ~Skybox() = default;

    void load();
    void display(glm::mat4 &projection, glm::mat4 &model, glm::mat4 &view);

private:
    Cubemap cubemap;
    std::vector<std::string> faces;
};
