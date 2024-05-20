#version 450 core

in vec2 tex;
uniform sampler2D Texture;

void main()
{
	vec4 texColour = texture(Texture,tex);
	if (texColour.a < 0.5)
		discard;
}