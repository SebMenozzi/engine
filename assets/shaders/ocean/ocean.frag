#version 330 core

// Inputs from Vertex shader
in vec3 fragPosition;

// Output
out vec4 fragColor;

void main()
{
    float value = fragPosition.y / 1.0;
    vec3 shadowColor = vec3(clamp(value, 0, 0.9));

    fragColor = vec4(mix(vec3(0.0353, 0.0941, 0.4078), shadowColor, 0.5), 0.8);
}
