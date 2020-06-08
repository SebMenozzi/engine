#version 330 core

in vec3 fragmentTextureCoords;

out vec4 fragmentColor;

uniform samplerCube skybox;

void main()
{
    fragmentColor = texture(skybox, fragmentTextureCoords);
}
