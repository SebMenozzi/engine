#version 330 core

uniform vec3 cameraPosition;

// Inputs from Vertex shader
in vec3 position;

// Output
out vec4 color;

float near = 0.1;
float far = 10.0;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float shadowValue = position.y / 1.0;
    vec3 shadowColor = vec3(clamp(shadowValue, 0, 0.9));

    float depthValue = 1.0 - linearizeDepth(gl_FragCoord.z) / far;

    vec3 final = mix(vec3(0.0549, 0.1255, 0.5255), shadowColor, 0.4);
    final = mix(vec3(0.5725, 0.7137, 1.0) * 0.5, final, depthValue);

    color = vec4(final, 1.0);
}
