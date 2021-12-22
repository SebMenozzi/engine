#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 view;
uniform mat4 projection;

in vec3 position[];
in vec3 normal[];

const float MAGNITUDE = 1.0;

void GenerateLine(int index)
{
    if (position[index].y >= 2)
    {
        gl_Position = gl_in[index].gl_Position;
        EmitVertex();
        gl_Position = gl_in[index].gl_Position + vec4(normal[index], 0.0) * MAGNITUDE;
        EmitVertex();
        EndPrimitive();
    }
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
