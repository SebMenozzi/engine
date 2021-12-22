#version 330 core

uniform float maxHeight;

// Inputs from Vertex shader
in vec3 fragPosition;

// Output
out vec4 fragColor;

float scale(float value, float oldMin, float oldMax, float newMin, float newMax)
{
    return ((newMax - newMin) * (value - oldMin) / (oldMax - oldMin)) + newMin;
}

void main()
{
    float value = fragPosition.y / maxHeight;
    float adjustedValue = scale(value, 0.0, 1.0, 0.0, 0.8);
    vec3 shadowColor = vec3(adjustedValue);
    float grayValue = 0.30 * shadowColor.x + 0.59 * shadowColor.y + 0.11 * shadowColor.z;

    fragColor = vec4(vec3(grayValue), 1.0);
}
