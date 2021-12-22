#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Sent to the Geometry shader
out vec3 position;
out vec3 normal;
out vec3 fragPosition;

void main()
{
    
    position = vec3(model * vec4(vertexPosition, 1.0));
    fragPosition = position;
    normal = mat3(transpose(inverse(model))) * vertexNormal;
    
    gl_Position = projection * view * vec4(position, 1.0);
}