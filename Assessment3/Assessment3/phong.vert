#version 450 core

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec2 vTex;
layout(location = 2) in vec3 vNor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projectedLightSpaceMatrix;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

uniform vec3 lightAmb;
uniform vec3 lightDiff;
uniform vec3 lightSpec;

out vec2 tex;
out vec3 nor;
out vec3 FragPosWorldSpace;
out vec4 FragPosProjectedLightSpace;
out vec3 ambIn;
out vec3 diffIn;
out vec3 specIn;
out float specHigh;

out vec3 lAmb;
out vec3 lDiff;
out vec3 lSpec;


void main()
{
	gl_Position = projection * view * model * vPos;
	tex = vTex.xy;
	nor = mat3(transpose(inverse(model))) * vNor;
	FragPosWorldSpace = vec3(model * vPos);
	FragPosProjectedLightSpace = projectedLightSpaceMatrix * model * vPos;

	ambIn = Ka;
	diffIn = Kd;
	specIn = Ks;
	specHigh = Ns;

	lAmb = lightAmb;
	lDiff = lightDiff;
	lSpec = lightSpec;
}
