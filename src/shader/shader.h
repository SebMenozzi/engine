#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif
#include <iostream>
#include <string>
#include <fstream>
#include "../utils/fileUtils.h"

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    bool loadShaders();
    GLuint getProgramID() const;

private:
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint programID;

    const char* vertexPath;
    const char* fragmentPath;

    std::string vertexSource;
    std::string fragmentSource;

    bool compileShader(GLuint &shaderId, GLenum shaderType, const GLchar* source);
    void linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};
