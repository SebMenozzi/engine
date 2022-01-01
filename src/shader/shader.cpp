#include "shader.h"

namespace shader
{
    // MARK: - Public

    Shader::Shader(
        const char* vertexPath, 
        const char* fragmentPath, 
        const char* geometryPath
    ): 
        vertexShaderID_(0),
        fragmentShaderID_(0),
        geometryShaderID_(0),
        programID_(0),
        vertexPath_(vertexPath), 
        fragmentPath_(fragmentPath), 
        geometryPath_(geometryPath)
    {}

    Shader::~Shader()
    {
        glDeleteShader(vertexShaderID_);
        glDeleteShader(fragmentShaderID_);
        if (geometryPath_ != nullptr)
            glDeleteShader(geometryShaderID_);
        
        glDeleteProgram(programID_);
    }

    bool Shader::load()
    {
        // Destroy old shaders if exist

        if (glIsShader(vertexShaderID_) == GL_TRUE)
            glDeleteShader(vertexShaderID_);

        if (glIsShader(fragmentShaderID_) == GL_TRUE)
            glDeleteShader(fragmentShaderID_);

        if (glIsShader(geometryShaderID_) == GL_TRUE)
            glDeleteShader(geometryShaderID_);

        if (glIsProgram(programID_) == GL_TRUE)
            glDeleteProgram(programID_);

        // Get content of each shader

        if (!utils::getFileContents(vertexPath_, vertexSource_))
            return false;

        if (!utils::getFileContents(fragmentPath_, fragmentSource_))
            return false;

        if (geometryPath_ != nullptr)
            if (!utils::getFileContents(geometryPath_, geometrySource_))
                return false;

        // Compile shaders

        if (!compileShader_(vertexShaderID_, GL_VERTEX_SHADER, vertexSource_.c_str()))
            return false;

        if (!compileShader_(fragmentShaderID_, GL_FRAGMENT_SHADER, fragmentSource_.c_str()))
            return false;

        if (geometryPath_ != nullptr)
            if (!compileShader_(geometryShaderID_, GL_GEOMETRY_SHADER, geometrySource_.c_str()))
                return false;

        // Link the program
        if (!linkProgram_())
            return false;

        return true;
    }

    GLuint Shader::getProgramID() const
    {
        return programID_;
    }

    void Shader::use()
    {
        glUseProgram(programID_);
    }

    void Shader::setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(programID_, name.c_str()), (int) value);
    }

    void Shader::setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(programID_, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(programID_, name.c_str()), value);
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(programID_, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(programID_, name.c_str()), x, y);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(programID_, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(programID_, name.c_str()), x, y, z);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(programID_, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(programID_, name.c_str()), x, y, z, w);
    }

    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(programID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(programID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(programID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    // MARK: - Private

    bool Shader::checkCompileErrors_(GLuint id, ShaderErrorType type, const GLchar* source)
    {
        GLint isSuccess = 0;
        GLint errorSize = 0;
        char error[1024];

        // Check compilation
        switch (type) {
        case COMPILATION:
            glGetShaderiv(id, GL_COMPILE_STATUS, &isSuccess);
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorSize);
            glGetShaderInfoLog(id, errorSize,NULL, error);
        case LINKING:
            glGetProgramiv(id, GL_LINK_STATUS, &isSuccess);
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &errorSize);
            glGetProgramInfoLog(id, errorSize, NULL, error);
        }

        // If there is an error
        if (isSuccess != GL_TRUE)
        {
            // Display the error
            if (source != nullptr)
                std::cerr << "from " << source << " => ";

            std::cerr << type << " : " << error << std::endl;

            glDeleteShader(vertexShaderID_);
            glDeleteShader(fragmentShaderID_);
            if (geometryPath_ != nullptr)
                glDeleteShader(geometryShaderID_);
            glDeleteProgram(programID_);

            return false;
        }

        return true;
    }

    bool Shader::compileShader_(GLuint &shaderId, GLenum shaderType, const GLchar* source)
    {
        // Create the shader
        shaderId = glCreateShader(shaderType);

        // Send the source to the shader
        glShaderSource(shaderId, 1, &source, nullptr);

        // Compile the shader
        glCompileShader(shaderId);

        // Verification of the compilation
        return checkCompileErrors_(shaderId, COMPILATION, source);
    }

    bool Shader::linkProgram_()
    {
        // Creation of the program
        programID_ = glCreateProgram();

        // Associate the shaders
        glAttachShader(programID_, vertexShaderID_);
        glAttachShader(programID_, fragmentShaderID_);
        if (geometryPath_ != nullptr)
            glAttachShader(programID_, geometryShaderID_);

        // Link the program
        glLinkProgram(programID_);

        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertexShaderID_);
        glDeleteShader(fragmentShaderID_);
        if (geometryPath_ != nullptr)
            glDeleteShader(geometryShaderID_);

        // Verification of the linking
        return checkCompileErrors_(programID_, LINKING, nullptr);
    }
}