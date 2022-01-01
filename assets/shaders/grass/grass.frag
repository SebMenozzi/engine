#version 330 core

uniform sampler2D grassTexture;

in vec2 uv;

out vec4 color;

void main()
{
    vec4 textureColor = texture(grassTexture, uv);
	if (textureColor.a < 0.05) discard;

    color = textureColor;
}
