#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 view;
uniform mat4 projection;

in vec3 normal[];

const float lineLength = 0.5;

void generateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(normal[index], 0.0) * lineLength;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generateLine(0);
    generateLine(1);
    generateLine(2);
}
