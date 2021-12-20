#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <vector>

#include "shader.h"
#include "types.h"

class Heightmap
{
public:
    Heightmap(int size, float step, const char* vertexPath, const char* fragmentPath);
    ~Heightmap();
    void circlesAlgorithm(int iterations);
    void load();
    void display(glm::mat4 &projection, glm::mat4 &modelView);
private:
    std::vector<std::vector<int> > heights;
    int size;
    float step;
    int minHeight;
    int maxHeight;

    shader::Shader shader;
    std::vector<vertex> vertices;
    std::vector<vertex> colors;

    GLuint vaoID;
    GLuint vboID;

    bool outOfBound(int x, int z);
    void addTile(int x, int z);
    int getHeight(int x, int z);
    void findMinMaxHeight();
};
