#version 330 core

in vec3 fragmentNormal;
in vec3 fragmentPosition;

out vec4 color;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(fragmentPosition - cameraPosition);
    vec3 R = refract(I, normalize(fragmentNormal), ratio);
    color = vec4(texture(skybox, R).rgb, 1.0);
}
