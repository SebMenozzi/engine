#pragma once

#include <vector>

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void load();
    void render();

protected:
    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<float> normals;

    GLuint vaoID;
    GLuint vboID;
};
