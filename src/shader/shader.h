#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include "../utils/fileUtils.h"

class Shader
{
public:
    /**
     * @brief Define the shader object with vertex and fragment files
     * @param void
     * @return void
    */
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    /**
     * @brief Delete the vertex / fragment shaders and the program
     * @param void
     * @return void
    */
    ~Shader();

    /**
     * @brief Load the shader object (Compilation and Linking)
     * @param void
     * @return success
    */
    bool load();

    /**
     * @brief Retrieve the program ID
     * @param void
     * @return GLuint
    */
    GLuint getProgramID() const;

    /**
     * @brief Use the shader
     * @param void
     * @return void
    */
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) ;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint geometryShaderID;
    GLuint programID;

    const char* vertexPath;
    const char* fragmentPath;
    const char* geometryPath;

    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource;

    bool compileShader(GLuint &shaderId, GLenum shaderType, const GLchar* source);
    bool linkProgram();
    bool checkCompileErrors(GLuint id, std::string type, const GLchar* source);
};
