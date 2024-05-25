#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vTex;
layout(location = 2) in vec3 vNor;

// out float Height;
//out vec3 Position;
out vec2 tex;
out vec3 nor;
out vec3 FragPosWorldSpace;
out vec4 FragPosProjectedLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projectedLightSpaceMatrix;

void main()
{
    // Height = aPos.y;
    //Position = (view * model * vec4(aPos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    tex = vTex.xy;
    nor = mat3(transpose(inverse(model))) * vNor;
	FragPosWorldSpace = vec3(model * vec4(aPos, 1.0));
	FragPosProjectedLightSpace = projectedLightSpaceMatrix * model * vec4(aPos, 1.0);
}