#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTextureCoords;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentTextureCoords;
out vec2 fragmentViewPosition;

uniform vec3 worldPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vectorOffset = vec4(worldPosition, 0.0);
    vec4 vertex = vec4(vertexPosition, 1.0);
    fragmentPosition = vec3(model * (vertex + vectorOffset));
    fragmentTextureCoords = vertexTextureCoords;
    fragmentNormal = mat3(transpose(inverse(model))) * vertexNormal;
    gl_Position = projection * view * vec4(fragmentPosition, 1.0);
}
