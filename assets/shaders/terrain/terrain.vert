#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float size;
uniform float scale;
uniform float displacementScale;
uniform sampler2D terrainTexture;

out vec3 position;
out vec3 normal;
out vec2 uv;

void main()
{
    vec3 scaledPosition = scale * vertexPosition.xyz;
    uv = vertexPosition.xz / size;

    vec2 uv1 = uv + vec2(scale, 0);
    vec2 uv2 = uv + vec2(0, scale);

    float height0 = texture(terrainTexture, uv).r * displacementScale;
    float height1 = texture(terrainTexture, uv1).r * displacementScale;
    float height2 = texture(terrainTexture, uv2).r * displacementScale;
    
    vec3 p0 = vec3(uv.x, height0, uv.y);
    vec3 p1 = vec3(uv1.x, height1, uv1.y);
    vec3 p2 = vec3(uv2.x, height2, uv2.y);
    
    vec3 v0 = p1 - p0;
    vec3 v1 = p2 - p0;

    normal = normalize(cross(v1, v0));

    float displacement = height0 * displacementScale;
    position = vec3(model * (vec4(scaledPosition + vec3(0, displacement, 0), 1.0)));
    
    gl_Position = projection * view * vec4(position, 1.0);
}