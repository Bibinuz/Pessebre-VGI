#version 330 core

#define NUM_LIGHTS 10

struct Light{
	bool sw_light;
	vec3 lightPos;
	vec4 lightColor;
	int tipus;
	float intensitat;
	sampler2D depthMap;
	mat4 lightSpaceMatrix;
};

out vec4 FragColor;

in vec3 crntPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec3 camPos;

uniform int numLights;
uniform Light lights[NUM_LIGHTS];

//Globals
vec4 FragPosLightSpace;
float a = 0.01;
float b = 0.01;
float c = 1;
float ambient = 0.1f;
float specularLight = 0.50f;
float inner_cone = 0.95f;
float outer_cone = 0.9f;

float diffuse;
float specular;
vec3 normalSup;
vec3 lightDirection;

float ShadowCalculation(int i)
{
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0)
		return 0.0;
	float closestDepth = texture(lights[i].depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normalSup, lightDirection)), 0.0005);
	float shadow = currentDepth-0.0005 > closestDepth ? 1.0 : 0.0;
	
	return shadow;

}

vec4 calculLightAmount(float inten, int i)
{
	FragPosLightSpace = lights[i].lightSpaceMatrix * vec4(crntPos, 1.0);
	float shadow = ShadowCalculation(i);
	vec4 lightAmount;
	lightAmount =  texture(diffuse0, texCoord) * (diffuse*inten*(1.0-shadow)+ambient) + inten * (1.0-shadow) * specular*0.2;
	lightAmount = lightAmount*lights[i].intensitat;

	return lightAmount;
}

float PointShadowCalculation(int i)
{
	FragPosLightSpace = lights[i].lightSpaceMatrix * vec4(crntPos, 1.0);
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0)
		return 0.0;
	float closestDepth = texture(lights[i].depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normalSup, lightDirection)), 0.0005);
	float shadow = currentDepth-0.0005 > closestDepth ? 1.0 : 0.0;

	return shadow;

}

vec4 calculPointLightAmount(float inten, int i)
{
	vec4 lightAmount;
	float shadow = 0.0;
	for(int j = 0; j<6;j++)
	{
		shadow += PointShadowCalculation(i+j);
	}
	shadow = shadow > 0.9 ? 1.0 : 0.0;
	lightAmount =  texture(diffuse0, texCoord) * (diffuse*inten*(1.0-shadow)+ambient) + inten * (1.0-shadow) * specular*0.2;
	lightAmount = lightAmount*lights[i].intensitat;
	return lightAmount;
}

vec4 pointLight(int i)
{ 	
	vec3 lightVec = lights[i].lightPos-crntPos;
	float dist = length(lightVec);
	
	float inten = 1.0f/(a*dist*dist + b*dist + c);
	
	vec4 lightAmount = calculPointLightAmount(inten, i);

	return lightAmount;

}

vec4 direcLight(int i)
{	
	return calculLightAmount(1, i);
}

vec4  spotLight(int i)
{
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten =  clamp((angle-outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f); 
	
	return calculLightAmount(inten, i);
}

void CalculComponentsPrincipals(int i)
{
		normalSup = normalize(normal);
		lightDirection = normalize(lights[i].lightPos - crntPos);	
		diffuse = max(dot(normalSup, lightDirection), 0.0f);
	
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 reflectionDirection = reflect(-lightDirection, normalSup);
		float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
		specular = specAmount * specularLight;
}


void main()
{
	FragColor = vec4(0, 0, 0, 0);


	for(int i = 0; i < numLights; i++)
	{
		if(lights[i].sw_light)
		{
			CalculComponentsPrincipals(i);
			switch(lights[i].tipus)
			{
				case 0:
					FragColor+=pointLight(i);
					i+=5;
					break;
				case 1:
					FragColor+=spotLight(i);
					break;
				case 2:
					FragColor+=direcLight(i);
					break;
				default:
					break;
			}
		}
	}
}