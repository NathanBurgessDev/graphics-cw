#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPosWorldSpace;
in vec4 FragPosProjectedLightSpace;
in vec3 ambIn;
in vec3 diffIn;
in vec3 specIn;
in float specHigh;

in vec3 lAmb;
in vec3 lDiff;
in vec3 lSpec;



uniform sampler2D Texture;
uniform sampler2D shadowMap;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

vec3 calculateDiffuse(vec3 Nnor,vec3 NToLight);
vec3 calculateSpecular(vec3 NrefLight, vec3 NcamDirection);
float calculateAttenuation();

vec3 CalculateDirectionIllumination(vec3 colour, vec3 Nnor);
vec3 CalculatePositionalIllumination(vec3 colour);
vec3 CalculateSpotIllumination(vec3 colour);
float shadowOnFragment(vec4 FragPosProjectedLightSpace);


void main()
{
	vec4 texColour = texture(Texture, tex);
	vec3 Nnor = normalize(nor);
	vec3 phong = CalculateDirectionIllumination(texColour.rgb, Nnor);
	if (texColour.a < 0.1)
		discard;
	fColour = vec4(phong, texColour.a);
}

vec3 calculateDiffuse(vec3 Nnor, vec3 NToLight){
	float diff = max(dot(Nnor, NToLight), 0);

	return vec3((diff * diffIn) * lDiff);
}

vec3 calculateSpecular(vec3 NrefLight, vec3 NcamDirection){


	float spec = pow(max(dot(NcamDirection, NrefLight), 0), specHigh);
	return vec3((spec * specIn )* lSpec);
}

float calculateAttenuation(){
	float constAttenuation = 1.0;
	float linearAttenuation = 0.05;
	float quadAttenuation = 0.002;

	float fragLightDistance = distance(FragPosWorldSpace, lightPos);

	return (1 / (constAttenuation + (linearAttenuation * fragLightDistance) + (quadAttenuation * pow(fragLightDistance, 2))));
}

vec3 CalculateDirectionIllumination(vec3 colour, vec3 Nnor) {
	vec3 ambient = (lAmb * 0.15);

	vec3 NtoLight = -normalize(lightDirection); 

	vec3 diffuse = calculateDiffuse(Nnor, NtoLight);

	vec3 Nfrom_light = normalize(lightDirection);
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);

	vec3 specular = calculateSpecular(NrefLight,NcamDirection);

	float shadow = shadowOnFragment(FragPosProjectedLightSpace);
	vec3 phong = (ambient + (1.f - shadow) * (diffuse + specular)) * colour;

	return phong;
}

vec3 CalculatePositionalIllumination(vec3 colour) {

	vec3 ambient = (lightColour * ambIn);

	// Diffuse
	vec3 Nnor = normalize(nor);
	vec3 NToLight = normalize(lightPos - FragPosWorldSpace);

	vec3 diffuse = calculateDiffuse(Nnor,NToLight);


	vec3 NFromLight = -NToLight;
	vec3 NrefLight = reflect(NFromLight, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);


	// Specular 
	// Should be set by uniform
	vec3 specular = calculateSpecular(NrefLight, NcamDirection);

	// Attenuation
	float attenuation = calculateAttenuation();

	float shadow = shadowOnFragment(FragPosProjectedLightSpace);
	vec3 phong = ((ambient + (1.f - shadow) * (diffuse + specular)) * attenuation) * colour;

	return phong;

}

vec3 CalculateSpotIllumination(vec3 colour) {
	vec3 ambient = (lightColour * ambIn);

	vec3 Nnor = normalize(nor);
	vec3 NToLight = normalize(lightPos - FragPosWorldSpace);
	vec3 diffuse = calculateDiffuse(Nnor, NToLight);

	vec3 NFromLight = -NToLight;
	vec3 NrefLight = reflect(NFromLight, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);


	vec3 specular = calculateSpecular(NrefLight, NcamDirection);

	float attenuation = calculateAttenuation();

	float cutoffAngle = 15.0;

	float phi = cos(radians(cutoffAngle));
	vec3 NSpotDir = normalize(lightDirection);
	float theta = dot(NFromLight, NSpotDir);

	float shadow = shadowOnFragment(FragPosProjectedLightSpace);

	vec3 phong;
	if (theta > phi) {
		phong = ((ambient + (1.f - shadow) * (diffuse + specular)) * attenuation) * colour;
	}
	else {
		phong = (ambient * attenuation * colour);
	}

	return phong;

}

float shadowOnFragment(vec4 FragPosProjectedLightSpace) {
	vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
	vec3 ss = (ndc + 1) * 0.5;

	float fragDepth = ss.z;

	float shadow = 0.f;

	vec3 Nnor = normalize(nor);
	vec3 NToLight = normalize(-lightDirection);
	float bias = max(0.005 * (1.0 - dot(Nnor, NToLight)), 0.005);
	//float bias = 0.005;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, ss.xy + vec2(x, y) * texelSize).r;
			shadow += (fragDepth - bias) > pcfDepth ? .95 : 0.0;
		}
	}
	shadow /= 9.0;

	//shadow = fragDepth > (litDepth + bias) ? 1.0 : 0.0;

	if (fragDepth > 1)
		shadow = 0.f;

	return shadow;
}