#pragma once

#include <vector>

#include "./mesh.h"
#include "../texture/texture.h"
#include "../types.h"
#include "../camera/camera.h"

class TexturedMesh : public Mesh
{
public:
    TexturedMesh(float size, const char* vertexPath, const char* fragmentPath, const char* diffusePath, const char* specularPath);
    void load();
    void display(
    glm::mat4 &projection,
    glm::mat4 &view,
    glm::mat4 &model,
    Camera &camera,
    std::vector<DirLight> dirLights,
    std::vector<PointLight> pointLights,
    std::vector<SpotLight> spotLights);

protected:
    Texture diffuse;
    Texture specular;

    std::vector<float> textureCoord;
};
