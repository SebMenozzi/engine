#version 330 core

in vec3 position;
in vec3 normal;

uniform vec3 sunPosition;
uniform vec3 cameraPosition;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

out vec4 color;

void main()
{
    vec3 sunDirection = normalize(sunPosition - position);
    float sunDistance = length(sunPosition - position);

    float sunDensity = (1.0 * sunDistance + 1.0 * (sunDistance * sunDistance));
    float sunAttenuation = 1.0 / sunDensity;

    vec3 sunColor = sunAttenuation * vec3(0.9882, 1.0, 0.8039);

    float ambientStrength = 0.1;
    vec3 ambientFinalColor = ambientStrength * sunColor * ambientColor.rgb;

    vec3 diffuseFinalColor = max(dot(normal, sunDirection), 0.0) * sunColor * diffuseColor.rgb;

    float specularS = 0;
    float shininess = 16;
    vec3 cameraDirection = normalize(cameraPosition - position);
    vec3 reflectDirection = reflect(sunDirection, normal);
    vec3 specularFinalColor = specularS * pow(max(dot(cameraDirection, reflectDirection), 0.0), shininess) * sunColor * specularColor;

    color = vec4(diffuseColor.rgb, 1.0);
}
