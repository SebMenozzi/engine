#pragma once

#include <vector>
#include <math.h>

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif

#include "../mesh.h"

class UVSphere: public Mesh
{
    public:
        UVSphere(float radius, int sectorCount, int stackCount);
        ~UVSphere();

        void load();
        void render();

    protected:
        float radius;
        int sectorCount;
        int stackCount;

        std::vector<float> textureCoord;
        std::vector<unsigned int> indices;

        GLuint eboID;
};
