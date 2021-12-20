#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "file_utils.h"

namespace shader
{
    enum ShaderErrorType {
        COMPILATION,
        LINKING
    };

    class Shader
    {
        public:
            Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
            ~Shader();

            bool load();
            GLuint getProgramID() const;

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
            bool compileShader_(GLuint &shaderId, GLenum shaderType, const GLchar* source);
            bool linkProgram_();
            bool checkCompileErrors_(GLuint id, ShaderErrorType type, const GLchar* source);

            GLuint vertexShaderID_;
            GLuint fragmentShaderID_;
            GLuint geometryShaderID_;
            GLuint programID_;

            const char* vertexPath_;
            const char* fragmentPath_;
            const char* geometryPath_;

            std::string vertexSource_;
            std::string fragmentSource_;
            std::string geometrySource_;
    };
}