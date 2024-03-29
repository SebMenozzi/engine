#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

void main()
{
    vec3 diffuse = texture(diffuseTexture, uv).rgb;

    color = vec4(diffuse.rgb, 1.0);
}