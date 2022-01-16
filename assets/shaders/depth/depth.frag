#version 330 core

in vec2 uv;

uniform sampler2D depthMapTexture;

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
    //float depthValue = texture(depthMapTexture, uv).r;
    float depthValue = linearizeDepth(gl_FragCoord.z) / far;

    color = vec4(vec3(depthValue), 1.0);
}