#version 410 core

uniform mat4 view;
uniform sampler2DArray depthMapsTexture;
uniform vec3 sunDirection;
uniform float farPlane;

uniform mat4 lightSpaceMatrices[16];
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;

// Inputs from Vertex shader
in vec3 position;
in vec3 normal;

// Output
out vec4 color;

float near = 0.1;
float far = 10.0;

float linear(float value, float min, float max)
{
    return (min * max) / (max + min - value * (max - min));
}

float shadowCalculation(vec3 position)
{
    vec4 positionViewSpace = view * vec4(position, 1.0);
    float depthValue = abs(positionViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
        layer = 0;

    vec4 positionLightSpace = lightSpaceMatrices[layer] * vec4(position, 1.0);
    vec3 coords = positionLightSpace.xyz / positionLightSpace.w;
    coords = coords * 0.5 + 0.5;

    if (coords.z > 1.0)
        return 0.0;

    float bias = max(0.05 * (1.0 - dot(normal, sunDirection)), 0.005);
    if (layer == cascadeCount)
        bias *= 1 / (farPlane * 0.5f);
    else
        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(depthMapsTexture, 0));

    for (int x = -4; x <= 4; ++x)
    {
        for (int y = -4; y <= 4; ++y)
        {
            float closestDepth = texture(depthMapsTexture, vec3(coords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (coords.z - bias) > closestDepth ? 0.01 : 0.0; 
        }
    }
        
    return shadow;
}

void main()
{

    float heightDepthValue = position.y / 0.5;
    vec3 heightDepthColor = vec3(heightDepthValue);

    float fogValue = 1.0 - 2.0 * linear(gl_FragCoord.z * 2.0 - 1.0, near, far) / far;

    vec3 oceanColor = mix(vec3(0.5725, 0.7137, 1.0) * 0.5, vec3(0.0549, 0.1255, 0.5255), fogValue);
    oceanColor = mix(heightDepthColor, oceanColor, 0.5);

    float shadow = clamp(shadowCalculation(position), 0.0, 0.4);

    vec3 oceanColorWithShadow = (1.0 - shadow) * oceanColor;

    color = vec4(oceanColorWithShadow, 1.0);
}
