#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;

// Sent to the Geometry shader
out vec3 position;

void main()
{
    position = vec3(model * vec4(vertexPosition, 1.0));
    gl_Position = vec4(position, 1.0);
}
