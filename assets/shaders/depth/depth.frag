#version 330 core

in vec4 position;

uniform vec3 sunPosition;
uniform float farPlane;

void main()
{
    float sunDistance = length(position.xyz - sunPosition);
    sunDistance = sunDistance / farPlane;

    gl_FragDepth = sunDistance;
}