#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPosWorldSpace;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform sampler2D Texture;

float CalculateDirectionIllumination();
float CalculatePositionalIllumination();
float CalculateSpotIllumination();

void main()
{
	float phong = CalculatePositionalIllumination();
	vec4 texColour = texture(Texture,tex);
	if (texColour.a < 0.1)
		discard;
	fColour = vec4(phong * vec3(texColour.x,texColour.y,texColour.z) * lightColour, texColour.a);
}

float CalculateDirectionIllumination(){
	float ambient = 0.1;

	vec3 Nnor = normalize(nor);
	vec3 Nto_light = -normalize(lightDirection);
	float diffuse = max(dot(Nnor, Nto_light),0);

	vec3 Nfrom_light = normalize(lightDirection);
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace; 
	vec3 NcamDirection = normalize(camDirection);
	float specular = pow(max(dot(NcamDirection, NrefLight),0),128);

	float phong = ambient + diffuse + specular;

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
