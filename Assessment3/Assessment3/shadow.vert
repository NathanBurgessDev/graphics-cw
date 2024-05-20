#version 450 core

layout (location = 0) in vec4 vPos;
layout(location = 1) in vec3 vTex;

uniform mat4 projectedLightSpaceMatrix;
uniform mat4 model;

out vec2 tex;

void main()
{
	tex = vTex.xy;
	gl_Position = projectedLightSpaceMatrix * model * vPos;
}