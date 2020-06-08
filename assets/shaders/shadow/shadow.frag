#version 330 core

out vec4 fragmentColor;

in VS_OUT {
    vec3 fragmentPosition;
    vec3 fragmentNormal;
    vec2 fragmentTextureCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform float farPlane;
uniform bool shadows;

float ShadowCalculation(vec3 fragmentPosition)
{
    // get vector between fragment position and light position
    vec3 fragmentToLight = fragmentPosition - lightPosition;
    // ise the fragment to light vector to sample from the depth map
    float closestDepth = texture(depthMap, fragmentToLight).r;
    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= farPlane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragmentToLight);

    // test for shadows
    float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    // display closestDepth as debug (to visualize depth cubemap)
    //fragmentColor = vec4(vec3(closestDepth / farPlane), 1.0);

    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.fragmentTextureCoords).rgb;
    vec3 normal = normalize(fs_in.fragmentNormal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDirection = normalize(lightPosition - fs_in.fragmentPosition);
    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDirection = normalize(viewPosition - fs_in.fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = 0.0;
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    spec = pow(max(dot(normal, halfwayDirection), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
    float shadow = shadows ? ShadowCalculation(fs_in.fragmentPosition) : 0.0;
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    fragmentColor = vec4(lighting, 1.0);
}
