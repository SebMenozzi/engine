#ifndef LIGHTCUBE_H_INCLUDED
#define LIGHTCUBE_H_INCLUDED

#include "../cube/cube.h"

class LightCube : public Cube
{
  public:
    LightCube(float size, const char* vertexPath, const char* fragmentPath, glm::vec3 color);
    ~LightCube();
    void display(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);

  private:
    glm::vec3 color;
};

#endif // LIGHTCUBE_H_INCLUDED
