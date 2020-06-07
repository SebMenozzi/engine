#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec3 fragmentTextureCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    fragmentTextureCoords = vertexPosition;
    gl_Position = projection * view * vec4(vertexPosition, 1.0);
}
