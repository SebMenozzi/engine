#version 330 core

uniform float maxHeight;

// Inputs from Vertex shader
in vec3 fragPosition;

// Output
out vec4 fragColor;

void main()
{
    float value = fragPosition.y / maxHeight;
    vec3 shadowColor = vec3(clamp(value, 0, 0.9));

    if (value == 0)
        fragColor = vec4(0);
    else
        fragColor = vec4(mix(vec3(0.0902, 0.2784, 0.0902), shadowColor, 0.5), 1.0);
}
