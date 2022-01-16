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
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMapsTexture, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r; 
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{

    float heightDepthValue = position.y / 1.0;
    vec3 heightDepthColor = vec3(clamp(heightDepthValue, 0, 0.9));

    float fogValue = 1.0 - 2.0 * linear(gl_FragCoord.z * 2.0 - 1.0, near, far) / far;

    vec3 oceanColor = mix(vec3(0.5725, 0.7137, 1.0) * 0.5, vec3(0.0549, 0.1255, 0.5255), fogValue);
    oceanColor = mix(oceanColor, heightDepthColor, 0.4);

    float shadow = clamp(shadowCalculation(position), 0.0, 0.5);

    vec3 oceanColorWithShadow = (1.0 - shadow) * oceanColor;

    color = vec4(oceanColorWithShadow, 1.0);
}
