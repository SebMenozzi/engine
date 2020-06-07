#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL3/gl.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shaders/shader.h"

class Cube
{
  public:
    Cube(float size, const char* vertexPath, const char* fragmentPath);
    ~Cube();

    void load();
    void display(glm::mat4 &projection, glm::mat4 &modelView);

  protected:
    Shader shader;
    float vertexIndices[36];
    float colorIndices[36];
    float vertices[24];
    float colors[9];

    GLuint vaoID;
    GLuint vboID;
    GLuint eboID;
    int verticesSizeBytes;
    int colorsSizeBytes;
    int indicesSizeBytes;
};

#endif
