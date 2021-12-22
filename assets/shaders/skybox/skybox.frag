#version 330 core

uniform samplerCube skybox;

// Inputs from Vertex shader
in vec3 fragmentPosition;

// Output
out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(skybox, fragmentPosition);
}
