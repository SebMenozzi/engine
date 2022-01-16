#version 330 core

uniform samplerCube depthMapTexture;
uniform vec3 sunPosition;
uniform vec3 sunColor;
uniform float farPlane;

// Inputs from Vertex shader
in vec3 position;

// Output
out vec4 color;

float near = 0.1;
float far = 10.0;

float linear(float value, float min, float max)
{
    return (min * max) / (max + min - value * (max - min));
}

float shadowCalculation(vec3 fragPosition)
{
    vec3 fragToLight = fragPosition - sunPosition;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMapTexture, fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= farPlane;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.1; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{

    float heightDepthValue = position.y / 1.0;
    vec3 heightDepthColor = vec3(clamp(heightDepthValue, 0, 0.9));

    float fogValue = 1.0 - 2.0 * linear(gl_FragCoord.z * 2.0 - 1.0, near, far) / far;

    vec3 oceanColor = mix(vec3(0.5725, 0.7137, 1.0) * 0.5, vec3(0.0549, 0.1255, 0.5255), fogValue);
    oceanColor = mix(oceanColor, heightDepthColor, 0.4);

    float shadow = clamp(shadowCalculation(position), 0.0, 0.3);

    vec3 oceanColorWithShadow = (1.0 - shadow) * oceanColor;

    color = vec4(oceanColorWithShadow, 1.0);
}
