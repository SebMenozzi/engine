#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 view;
uniform mat4 projection;

// Sent to the fragment shader
out vec3 fragmentPosition;

void main()
{
    fragmentPosition = vertexPosition;
    gl_Position = projection * view * vec4(vertexPosition, 1.0);
}
