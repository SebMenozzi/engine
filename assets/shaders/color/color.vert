#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

out vec3 fragmentColor;
out vec3 fragmentPosition;
out vec3 fragmentNormal;

uniform vec3 worldPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 vectorOffset = vec4(worldPosition, 0.0);
    vec4 vertex = vec4(vertexPosition, 1.0);
    
    fragmentColor = vertexColor;

    gl_Position = projection * view * model * (vertex + vectorOffset);
}
