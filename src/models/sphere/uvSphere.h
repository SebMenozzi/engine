#pragma once

#include <vector>

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../shader/shader.h"
#include "../../types.h"

class UVSphere
{
    public:
        UVSphere(float radius, int sectorCount, int stackCount, const char* vertexPath, const char* fragmentPath);
        ~UVSphere();

        void load();
        void display(glm::mat4 &projection, glm::mat4 &model, glm::mat4 &view);
        void setPosition(glm::vec3 position);

    protected:
        Shader shader;

        float radius;
        int sectorCount;
        int stackCount;

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> textureCoord;
        std::vector<unsigned int> indices;

        glm::vec3 position;

        GLuint vaoID;
        GLuint vboID;
        GLuint eboID;
};
