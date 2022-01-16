#version 330 core

uniform float maxHeight;
uniform samplerCube depthMapTexture;
uniform vec3 sunPosition;
uniform vec3 sunColor;
uniform float farPlane;

// Inputs from Vertex shader
in vec3 position;

// Output
out vec4 color;

float shadowCalculation(vec3 position)
{
    vec3 fragToLight = position - sunPosition;
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
    float value = position.y / maxHeight;
    vec3 heightColor = vec3(clamp(value, 0, 0.9));
    vec3 terrainColor = mix(vec3(0.0902, 0.2784, 0.0902), heightColor, 0.5);
    
    float shadow = clamp(shadowCalculation(position), 0.0, 0.3);

    vec3 terrainColorWithShadow = (1.0 - shadow) * terrainColor;

    color = vec4(terrainColorWithShadow, 1.0);
}
