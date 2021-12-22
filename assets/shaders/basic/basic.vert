#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 fragmentPosition = vec3(model * vec4(vertexPosition, 1.0));
    gl_Position = projection * view * vec4(fragmentPosition, 1.0);
}
