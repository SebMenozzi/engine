#version 330 core

uniform vec3 sunDirection;

// Inputs from Vertex shader
in vec3 position;
in vec3 normal;
in vec2 uv;

// Output
out vec4 color;

void main()
{
    float light = dot(-sunDirection, normal);
    vec3 terrainColor = vec3(0.5451, 0.4824, 0.349);

    color = vec4(terrainColor * (light * 0.5 + 0.5), 1.0);
}
