#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform sampler2D terrainTexture;

uniform mat4 model;

uniform float size;
uniform float scale;
uniform float displacementScale;

// Sent to the Geometry shader
out vec3 position;

void main()
{
    vec3 scaledPosition = scale * vertexPosition.xyz;
    vec2 uv = vertexPosition.xz / size;

    float height0 = texture(terrainTexture, uv).r * displacementScale;

    float displacement = height0 * displacementScale;
    position = vec3(model * (vec4(scaledPosition + vec3(0, displacement, 0), 1.0)));

    gl_Position = vec4(position, 1.0);
}
