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

float ShadowCalculation(int i, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0)
		return 0.0;
	float closestDepth = texture(lights[i].depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
	float shadow = currentDepth-0.0005 > closestDepth ? 1.0 : 0.0;

	return shadow;

}


vec4 calculLightAmount(float diffuse, float inten, float specular, int i, vec3 normal, vec3 lightDir)
{
	vec4 lightAmount;
	float shadow = ShadowCalculation(i, normal, lightDir);
	
	lightAmount =  texture(diffuse0, texCoord) * (diffuse*inten*(1.0-shadow)+ambient) + inten * (1.0-shadow) * specular*0.05;

	lightAmount = lightAmount*lights[i].intensitat;
	return lightAmount;
}

float PointShadowCalculation(int i, vec3 normal, vec3 lightDir)
{
	FragPosLightSpace = lights[i].lightSpaceMatrix * vec4(crntPos, 1.0);
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0)
		return 0.0;
	float closestDepth = texture(lights[i].depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
	float shadow = currentDepth-0.0005 > closestDepth ? 1.0 : 0.0;

	return shadow;

}

vec4 calculPointLightAmount(float diffuse, float inten, float specular, int i, vec3 normal, vec3 lightDir)
{
	vec4 lightAmount;
	float shadow = 0.0;
	for(int j = 0; j<6;j++)
	{
		shadow += PointShadowCalculation(i+j, normal, lightDir);
	}
	shadow = shadow > 0.9 ? 1.0 : 0.0;
	lightAmount =  texture(diffuse0, texCoord) * (diffuse*inten*(1.0-shadow)+ambient) + inten * (1.0-shadow) * specular*0.05;
	lightAmount = lightAmount*lights[i].intensitat;
	return lightAmount;
}

vec4 pointLight(int i)
{ 	
	vec3 lightVec = lights[i].lightPos-crntPos;
	float dist = length(lightVec);
	
	float inten = 1.0f/(a*dist*dist + b*dist + c);
	
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lights[i].lightPos - crntPos);	

	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	vec4 lightAmount = calculPointLightAmount(diffuse, inten, specular, i, normal, lightDirection);

	return lightAmount;

}

vec4 direcLight(int i)
{	
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lights[i].lightPos-crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return calculLightAmount(diffuse, 1, specular, i, normal, lightDirection);

}

vec4  spotLight(int i){
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lights[i].lightPos - crntPos);	
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten =  clamp((angle-outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f); 

	return calculLightAmount(diffuse, inten, specular, i, normal, lightDirection);
}

void main()
{
	FragColor = vec4(0, 0, 0, 0);


	for(int i = 0; i < numLights; i++)
	{
		if(lights[i].sw_light)
		{

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
					FragPosLightSpace = lights[i].lightSpaceMatrix * vec4(crntPos, 1.0);
					FragColor+=direcLight(i);
					break;
				default:
					break;
			}
		}
	}
}