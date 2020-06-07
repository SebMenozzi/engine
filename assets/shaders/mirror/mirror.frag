#version 330 core

in vec3 fragmentNormal;
in vec3 fragmentPosition;

out vec4 color;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(fragmentPosition - cameraPosition);
    vec3 R = reflect(I, normalize(fragmentNormal));
    color = vec4(texture(skybox, R).rgb, 1.0);
}
