#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shader/shader.h"

#include "../array3D.h"
#include <time.h>
#include "../types.h"
#include <vector>

class Chunk
{
public:
    Chunk(int size, int height, const char* vertexPath, const char* fragmentPath);
    ~Chunk();
    void setBlock(int x, int y, int z, int value);
    int getBlock(int x, int y, int z);
    void setPosition(glm::vec3 position);
    void fillRandom();
    void load();
    void display(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);
private:
    Array3D data;
    int size;
    int height;
    glm::vec3 position;

    Shader shader;
    std::vector<vertex> vertices;
    std::vector<vertex> colors;

    GLuint vaoID;
    GLuint vboID;

    bool outOfBound(int x, int y, int z);
    void addFace(int x, int y, int z, facePosition facing);
};
