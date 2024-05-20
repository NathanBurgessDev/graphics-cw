#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPosWorldSpace;
in vec4 FragPosProjectedLightSpace;

uniform sampler2D Texture;
uniform sampler2D shadowMap;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

vec3 CalculateDirectionIllumination(vec3 colour);
float CalculatePositionalIllumination();
float CalculateSpotIllumination();
float shadowOnFragment(vec4 FragPosProjectedLightSpace);

void main()
{
	vec4 texColour = texture(Texture,tex);
	vec3 phong = CalculateDirectionIllumination(texColour.rgb);
	if (texColour.a <0.1)
		discard;
	fColour = vec4(phong,texColour.a);
}

vec3 CalculateDirectionIllumination(vec3 colour){
	vec3 ambient = 0.1 * lightColour;

	vec3 Nnor = normalize(nor);
	vec3 Nto_light = -normalize(lightDirection);
	float diff = max(dot(Nnor, Nto_light),0);
	vec3 diffuse = diff * lightColour;

	vec3 Nfrom_light = normalize(lightDirection);
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace; 
	vec3 NcamDirection = normalize(camDirection);
	float spec = pow(max(dot(NcamDirection, NrefLight),0),128);
	vec3 specular = spec * lightColour;

	float shadow = shadowOnFragment(FragPosProjectedLightSpace);
	vec3 phong = (ambient + (1.f - shadow) * (diffuse + specular)) * colour;

	return phong;
}

float CalculatePositionalIllumination(){
	
	float ambient = 0.1;

	vec3 Nnor = normalize(nor);
	vec3 NToLight = normalize(lightPos - FragPosWorldSpace);
	float diffuse = max(dot(Nnor, NToLight),0);

	vec3 NFromLight = -NToLight;
	vec3 NrefLight = reflect(NFromLight, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace; 
	vec3 NcamDirection = normalize(camDirection);
	float specular = pow(max(dot(NcamDirection, NrefLight),0),128);

	float constAttenuation = 1.0;
	float linearAttenuation = 0.05;
	float quadAttenuation = 0.002;

	float fragLightDistance = distance(FragPosWorldSpace, lightPos);

	float attenuation = 1 / (constAttenuation + (linearAttenuation * fragLightDistance) + (quadAttenuation * pow(fragLightDistance,2)));

	float phong = (ambient + diffuse + specular) * attenuation;

	return phong;

}

float CalculateSpotIllumination(){
	float ambient = 0.1;

	vec3 Nnor = normalize(nor);
	vec3 NToLight = normalize(lightPos - FragPosWorldSpace);
	float diffuse = max(dot(Nnor, NToLight),0);

	vec3 NFromLight = -NToLight;
	vec3 NrefLight = reflect(NFromLight, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace; 
	vec3 NcamDirection = normalize(camDirection);
	float specular = pow(max(dot(NcamDirection, NrefLight),0),128);

	float constAttenuation = 1.0;
	float linearAttenuation = 0.05;
	float quadAttenuation = 0.002;
	float fragLightDistance = distance(FragPosWorldSpace, lightPos);
	float attenuation = 1 / (constAttenuation + (linearAttenuation * fragLightDistance) + (quadAttenuation * pow(fragLightDistance,2)));

	float cutoffAngle = 15.0;

	float phi = cos(radians(cutoffAngle));
	vec3 NSpotDir = normalize(lightDirection);
	float theta = dot(NFromLight,NSpotDir);

	float phong;
	if (theta > phi){
		phong = (ambient + diffuse + specular) * attenuation;
	}
	else{
	phong = ambient * attenuation;
	}

	return phong;

}

float shadowOnFragment(vec4 FragPosProjectedLightSpace){
	vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
	vec3 ss = (ndc+1)*0.5;

	float fragDepth = ss.z;

	float shadow = 0.f;

	float litDepth = texture(shadowMap,ss.xy).r;

	vec3 Nnor = normalize(nor);
	vec3 NToLight = normalize(-lightDirection);
	float bias = max(0.05 * (1.0 - dot(Nnor,NToLight)), 0.005);

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(shadowMap, ss.xy + vec2(x, y) * texelSize).r; 
			shadow += (fragDepth - bias) > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if (fragDepth > 1)
		shadow = 0.f;

	return shadow;
}