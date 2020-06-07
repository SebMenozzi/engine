#version 330 core

in vec3 fragmentTextureCoords;

out vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, fragmentTextureCoords);
}
