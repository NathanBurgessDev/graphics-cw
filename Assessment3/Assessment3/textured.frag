#version 450 core

in vec2 tex;


uniform sampler2D Texture;

out vec4 fragColour;

void main()
{
	fragColour = texture(Texture, tex);
}
