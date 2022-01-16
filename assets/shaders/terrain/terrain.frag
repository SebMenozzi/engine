#version 330 core

uniform float maxHeight;

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
        layer = cascadeCount;

    vec4 positionLightSpace = lightSpaceMatrices[layer] * vec4(position, 1.0);
    // perform perspective divide
    vec3 projCoords = positionLightSpace.xyz / positionLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth  > 1.0)
        return 0.0;

    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - dot(normal, sunDirection)), 0.005);
    if (layer == cascadeCount)
        bias *= 1 / (farPlane * 0.5f);
    else
        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(depthMapsTexture, 0));
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMapsTexture, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r; 
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    float value = position.y / maxHeight;
    vec3 heightColor = vec3(clamp(value, 0, 0.9));
    vec3 terrainColor = mix(vec3(0.0902, 0.2784, 0.0902), heightColor, 0.5);
    
    float shadow = clamp(shadowCalculation(position), 0.0, 0.5);

    vec3 terrainColorWithShadow = (1.0 - shadow) * terrainColor;

    color = vec4(terrainColorWithShadow, 1.0);
}
