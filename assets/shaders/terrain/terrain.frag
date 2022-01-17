#version 330 core

uniform float maxHeight;

uniform mat4 view;
uniform vec3 sunDirection;
uniform float farPlane;

uniform mat4 lightSpaceMatrices[16];
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;

uniform sampler2DArray depthMapsTexture;

// Inputs from Vertex shader
in vec3 position;
in vec3 normal;
in vec2 uv;

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
        layer = 0;

    vec4 positionLightSpace = lightSpaceMatrices[layer] * vec4(position, 1.0);
    vec3 coords = positionLightSpace.xyz / positionLightSpace.w;
    coords = coords * 0.5 + 0.5;

    /*
    if (coords.z > 1.0)
        return 0.0;
    */

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
            float closestDepth = texture(depthMapsTexture, vec3(coords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (coords.z + 0.1 - bias) > closestDepth ? 1.0 : 0.0; 
        }
    }

    return shadow;
}

void main()
{
    float shadow = shadowCalculation(position);
    vec3 terrainColorWithShadow = (1.0 - shadow) * vec3(1.0);

    color = vec4(terrainColorWithShadow, 1.0);

    /*
    float light = dot(sunDirection, normal);
    vec3 terrainColor = vec3(0.5451, 0.4824, 0.349);
    
    color = vec4(terrainColor * (light * 0.5 + 0.5), 1);
    */
}
