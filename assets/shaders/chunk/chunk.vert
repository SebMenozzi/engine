#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
//layout(location = 2) in vec3 vertexNormal;

uniform vec3 worldPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fragmentColor;

void main()
{
    vec4 vectorOffset = vec4(worldPosition, 0.0);
    vec4 vertex = vec4(vertexPosition, 1.0);

    gl_Position = projection * view * model * (vertex + vectorOffset);
    fragmentColor = vertexColor;
}
