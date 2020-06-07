#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;

uniform vec3 worldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vectorOffset = vec4(worldPosition, 0.0);
    vec4 vertex = vec4(vertexPosition, 1.0);

    fragmentNormal = mat3(transpose(inverse(model))) * vertexNormal;
    fragmentPosition = vec3(model * (vertex + vectorOffset));
    gl_Position = projection * view * vec4(fragmentPosition, 1.0);
}
