#version 410 core

uniform vec3 cameraPosition;
uniform vec3 sunDirection;

// Inputs from Vertex shader
in vec3 position;
in vec3 normal;

// Output
out vec4 color;

const vec3 WATER_LIGHTEST = vec3(0.1294, 0.1608, 0.6078);
const vec3 WATER_DARKEST = vec3(0.0039, 0.1294, 0.3647);
const vec3 SKY_COLOR = vec3(0.09, 0.33, 0.81);

float fresnel(float F0)
{
    float cosTheta = dot(normal, normalize(cameraPosition - position));
    
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 sky(vec3 e)
{
    return mix(
        vec3(0.451, 0.6157, 0.8), 
        mix(SKY_COLOR, 0.5 * SKY_COLOR, e.y), 
        smoothstep(-0.5, 0.25, e.y)
    );
}

void main()
{
    float lightValue = dot(sunDirection, normal);

    vec3 oceanColor = vec3(0.0549, 0.1255, 0.5255);
    
    vec3 finalColor = oceanColor * (lightValue * 0.2 + 0.5);

    float fresnelValue = clamp(fresnel(0.02), 0.0, 1.0) * 0.2; // 0.02 => Water

    vec3 reflected = sky(
        reflect(cameraPosition, normal)) * 0.5 + 5.0 * pow(max(dot(cameraPosition, reflect(normal, sunDirection)), 0.1
    ), 1.0);

    // indirect light from the sun through water
    vec3 refracted = WATER_DARKEST +  WATER_LIGHTEST * pow(0.4 * lightValue + 0.6, 1.0) * 0.1;
    refracted += WATER_LIGHTEST * 0.2 * (position.y - 1.0);

    color = vec4(mix(refracted, reflected, fresnelValue), 1.0);
}
