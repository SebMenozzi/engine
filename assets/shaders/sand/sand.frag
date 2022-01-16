#version 330 core

in vec2 uv;

uniform sampler2D sandTexture;
uniform int time;

out vec4 color;

void main()
{
    vec2 warpUV = 60.0 * uv;

    float dist = length(warpUV);
    float speed = 0.0001;
    warpUV = warpUV * 0.1 + 0.2 * vec2(cos(speed * time * 2.0 + dist), sin(speed * time * 2.0 - dist));

    vec3 warpedColor = texture(sandTexture, warpUV).xyz;
    float w = max(warpedColor.r, 0.85);
	
    vec2 offset = 0.01 * cos(warpedColor.rg * 3.14159);
    vec3 final = texture(sandTexture, uv + offset).rgb * vec3(0.8, 0.8, 1.5);
	final *= w * 1.2;

    color = vec4(mix(final, texture(sandTexture, uv + offset).rgb, 0.5),  1.0);
}