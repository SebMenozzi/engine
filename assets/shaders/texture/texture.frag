#version 330 core

#define NB_MAX_DIR_LIGHTS 4
#define NB_MAX_POINT_LIGHTS 4
#define NB_MAX_SPOT_LIGHTS 4

// Types
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Inputs
in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentTextureCoords;

// Output
out vec4 color;

uniform vec3 viewPosition;
uniform DirLight dirLights[NB_MAX_DIR_LIGHTS];
uniform PointLight pointLights[NB_MAX_POINT_LIGHTS];
uniform SpotLight spotLights[NB_MAX_SPOT_LIGHTS];
uniform Material material;

uniform int nbDirLights;
uniform int nbPointLights;
uniform int nbSpotLights;

// Function declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 normal = normalize(fragmentNormal);
    vec3 viewDir = normalize(viewPosition - fragmentPosition);

    vec3 result = vec3(0.0f, 0.0f, 0.0f); // black by default

    for(int i = 0; i < 1; i++)
    result += CalcDirLight(dirLights[i], normal, viewDir);

    for(int i = 0; i < 2; i++)
    result += CalcPointLight(pointLights[i], normal, viewDir);

    for(int i = 0; i < 0; i++)
    result += CalcSpotLight(spotLights[i], normal, viewDir);

    color = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragmentTextureCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragmentTextureCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragmentTextureCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragmentTextureCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragmentTextureCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragmentTextureCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragmentTextureCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragmentTextureCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragmentTextureCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
