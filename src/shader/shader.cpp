#include "shader.h"

/* Public methods */

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
    this->geometryPath = geometryPath;

    this->vertexShaderID = 0;
    this->fragmentShaderID = 0;
    if (this->geometryPath != nullptr)
        this->geometryShaderID = 0;
    this->programID = 0;
}

Shader::~Shader()
{
    glDeleteShader(this->vertexShaderID);
    glDeleteShader(this->fragmentShaderID);
    if (this->geometryPath != nullptr)
        glDeleteShader(this->geometryShaderID);
    glDeleteProgram(this->programID);
}

bool Shader::load()
{
    // Destroy old shaders

    if (glIsShader(this->vertexShaderID) == GL_TRUE)
        glDeleteShader(this->vertexShaderID);

    if (glIsShader(this->fragmentShaderID) == GL_TRUE)
        glDeleteShader(this->fragmentShaderID);

    if (glIsShader(this->geometryShaderID) == GL_TRUE)
        glDeleteShader(this->geometryShaderID);

    if (glIsProgram(this->programID) == GL_TRUE)
        glDeleteProgram(this->programID);

    // Get content of each shader

    if (getFileContents(this->vertexPath, this->vertexSource) == false)
        return false;

    if (getFileContents(this->fragmentPath, this->fragmentSource) == false)
        return false;

    if (this->geometryPath != nullptr)
        if (getFileContents(this->geometryPath, this->geometrySource) == false)
            return false;

    // Compile shaders

    if (compileShader(this->vertexShaderID, GL_VERTEX_SHADER, this->vertexSource.c_str()) == false)
        return false;

    if (compileShader(this->fragmentShaderID, GL_FRAGMENT_SHADER, this->fragmentSource.c_str()) == false)
        return false;

    if (this->geometryPath != nullptr)
        if (compileShader(this->geometryShaderID, GL_GEOMETRY_SHADER, this->geometrySource.c_str()) == false)
            return false;

    // Link the program
    if (linkProgram() == false)
        return false;

    return true;
}

GLuint Shader::getProgramID() const
{
    return this->programID;
}

void Shader::use()
{
    glUseProgram(this->programID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(this->programID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(this->programID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(this->programID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(this->programID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

/* Private methods */

bool Shader::checkCompileErrors(GLuint id, std::string type, const GLchar* source)
{
    GLint isSuccess = 0;
    GLint errorSize = 0;
    char* error;

    // Error of compilation
    if (type == "COMPILATION")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &isSuccess);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorSize);

        error = new char[errorSize + 1];
        glGetShaderInfoLog(id, errorSize, &errorSize, error);
    }
    // Error of linking
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &isSuccess);
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &errorSize);

        error = new char[errorSize + 1];
        glGetProgramInfoLog(id, errorSize, &errorSize, error);
    }

    // There is an error
    if (isSuccess != GL_TRUE)
    {
        error[errorSize] = '\0';

        // Display the error
        if (source != nullptr)
            std::cout << "from " << source << " => ";

        std::cout << type << " : " << error << std::endl;

        delete[] error;

        glDeleteShader(this->vertexShaderID);
        glDeleteShader(this->fragmentShaderID);
        glDeleteProgram(this->programID);

        return false;
    }

    return true;
}

bool Shader::compileShader(GLuint &shaderId, GLenum shaderType, const GLchar* source)
{
    // Create the shader
    shaderId = glCreateShader(shaderType);

    // Send the source to the shader
    glShaderSource(shaderId, 1, &source, nullptr);

    // Compile the shader
    glCompileShader(shaderId);

    // Verification of the compilation
    return checkCompileErrors(shaderId, "COMPILATION", source);
}

bool Shader::linkProgram()
{
    // Creation of the program
    this->programID = glCreateProgram();

    // Associate the shaders
    glAttachShader(this->programID, this->vertexShaderID);
    glAttachShader(this->programID, this->fragmentShaderID);

    if (this->geometryPath != nullptr)
        glAttachShader(this->programID, this->geometryShaderID);

    // Link the program
    glLinkProgram(this->programID);

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(this->vertexShaderID);
    glDeleteShader(this->fragmentShaderID);
    if (this->geometryPath != nullptr)
        glDeleteShader(this->geometryShaderID);

    // Verification of the linking
    return checkCompileErrors(this->programID, "LINKING", nullptr);
}
