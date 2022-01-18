#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float size;
uniform float scale;
uniform int time;
uniform float displacementScale;

out vec3 position;
out vec3 normal;
out vec2 uv;

const float speed = 0.0002;

float quintic(float x)
{
 	return x * x * x * (6.0 * x * x - 15.0 * x + 10.0);
}

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    float fac = 43758.5453123;
    vec2 m = vec2(100., 1.);
    vec2 o = vec2(1., 0.);
    
    float n00 = fract(fac * sin(dot((i + o.yy), m)));
    float n01 = fract(fac * sin(dot((i + o.xy), m)));
    float n10 = fract(fac * sin(dot((i + o.yx), m)));
    float n11 = fract(fac * sin(dot((i + o.xx), m)));
    
    float fx = quintic(f.x);
    float px0 = mix(n00, n01, fx);
    float px1 = mix(n10, n11, fx);
    
    return mix(px0, px1, quintic(f.y));
}

mat2 rotate = mat2(0.8, -0.6, 0.6, 0.8);

float ocean(vec2 p)
{
    float f = 0.0;
    
    vec2 v01 = vec2( 2.,  0.) * time * speed;
    vec2 v02 = vec2( 2.,  4.) * time * speed;
    vec2 v03 = vec2( 5.,  5.) * time * speed;
    vec2 v04 = vec2( 6.,  3.) * time * speed;
    vec2 v05 = vec2( 8.,  5.) * time * speed;
    vec2 v06 = vec2(16., 16.) * time * speed;
    
    f += 0.50000 * noise(p * 1.0   + v01); 
    p *= rotate;
    f += 0.25000 * noise(p * 2.01  + v03); 
    p *= rotate;
    f += 0.06250 * noise(p * 8.09  + v04); 
    p *= rotate;
    f += 0.03215 * noise(p * 15.98 + v05); 
    p *= rotate;
    f += 0.01608 * noise(p * 32.93 + v06);
    p *= rotate;

    return f;
}


float getHeight(vec2 pos)
{
    return displacementScale * ocean(pos);
}


void main()
{
    vec3 scaledPosition = scale * vertexPosition.xyz;
    uv = vertexPosition.xz / size;

    float height0 = getHeight(scaledPosition.xz);
    float height1 = getHeight(vec2(scaledPosition.x + scale, scaledPosition.z));
    float height2 = getHeight(vec2(scaledPosition.x, scaledPosition.z + scale));
    
    vec3 p0 = vec3(scaledPosition.x, height0, scaledPosition.z);
    vec3 p1 = vec3(scaledPosition.x + scale, height1, scaledPosition.z);
    vec3 p2 = vec3(scaledPosition.x, height2, scaledPosition.z + scale);
    
    vec3 v0 = p1 - p0;
    vec3 v1 = p2 - p0;

    normal = normalize(cross(v1, v0));

    position = vec3(model * (vec4(scaledPosition + vec3(0, height0, 0), 1.0)));
    
    gl_Position = projection * view * vec4(position, 1.0);
}
