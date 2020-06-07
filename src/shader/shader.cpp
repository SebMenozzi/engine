#include "shader.h"

/* Public methods */

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;

    this->vertexShaderID = 0;
    this->fragmentShaderID = 0;
    this->programID = 0;
}

Shader::~Shader()
{
    glDeleteShader(this->vertexShaderID);
    glDeleteShader(this->fragmentShaderID);
    glDeleteProgram(this->programID);
}

bool Shader::loadShaders()
{
    /* Destroy old shaders */

    if (glIsShader(this->vertexShaderID) == GL_TRUE)
        glDeleteShader(this->vertexShaderID);

    if(glIsShader(this->fragmentShaderID) == GL_TRUE)
        glDeleteShader(this->fragmentShaderID);

    if(glIsProgram(this->programID) == GL_TRUE)
        glDeleteProgram(this->programID);

    /* Get content of each shader */

    if (getFileContents(this->vertexPath, this->vertexSource) == false)
        return false;

    if (getFileContents(this->fragmentPath, this->fragmentSource) == false)
        return false;

    /* Compile shaders */

    if (compileShader(this->vertexShaderID, GL_VERTEX_SHADER, this->vertexSource.c_str()) == false)
        return false;

    if (compileShader(this->fragmentShaderID, GL_FRAGMENT_SHADER, this->fragmentSource.c_str()) == false)
        return false;

    // Link the program
    linkProgram(this->vertexShaderID, this->fragmentShaderID);

    return true;
}

GLuint Shader::getProgramID() const
{
    return this->programID;
}

/* Private methods */

bool Shader::compileShader(GLuint &shaderId, GLenum shaderType, const GLchar* source)
{
    // Create the shader
    shaderId = glCreateShader(shaderType);

    // Send the source to the shader
    glShaderSource(shaderId, 1, &source, nullptr);

    // Compile the shader
    glCompileShader(shaderId);

    // Verification of the compilation
    GLint isSuccess = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isSuccess);

    // There is an error
    if (isSuccess != GL_TRUE)
    {

        GLint errorSize = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorSize);

        char* error = new char[errorSize + 1];
        glGetShaderInfoLog(shaderId, errorSize, &errorSize, error);
        error[errorSize] = '\0';

        // Display the error
        std::cout << error << std::endl;

        delete[] error;
        glDeleteShader(shaderId);

        return false;
    }

    return true;
}

void Shader::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
    // Creation of the program
    this->programID = glCreateProgram();

    // Associate the shaders
    glAttachShader(this->programID, vertexShaderID);
    glAttachShader(this->programID, fragmentShaderID);

    // Link the program
    glLinkProgram(this->programID);
}
