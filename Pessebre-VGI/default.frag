#version 330 core

#define NUM_LIGHTS 10

struct Light{
	bool sw_light;
	vec3 lightPos;
	vec4 lightColor;
	int tipus;
	float intensitat;
	sampler2D depthMap;
	samplerCube depthCubeMap;

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
float a = 0.01;
float b = 0.01;
float c = 1.0;
float ambient = 0.3;
float specularLight = 0.50;
float inner_cone = 0.95;
float outer_cone = 0.9;
float farPlane = 25.0;

float DirecShadowCalculation(vec4 FragPosLightSpace, int i, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(lights[i].depthMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
	float shadow = currentDepth-bias > closestDepth ? 1.0 : 0.0;
	return shadow;

}

float PointShadowCalculation(int i, vec3 normal, vec3 lightDir)
{
	vec3 fragToLight = crntPos - lights[i].lightPos;
	float currentDepth = length(fragToLight);
	float closestDepth = texture(lights[i].depthCubeMap, fragToLight).r;
	closestDepth *= farPlane;
	float bias = max(0.5 * (1.0 - dot(normal, lightDir)), 0.0005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;

}

vec4 DirecCalculLightAmount(float diffuse, float specular, int i, vec3 normal, vec3 lightDir)
{
	vec4 lightAmount;
	vec4 FragPosLightSpace = lights[i].lightSpaceMatrix * vec4(crntPos, 1.0);
	float shadow = DirecShadowCalculation(FragPosLightSpace, i, normal, lightDir);

	lightAmount =  texture(diffuse0, texCoord) * (diffuse*(1.0-shadow)+ambient) + (1.0-shadow) * specular*0.2   ;

	lightAmount = lightAmount*lights[i].intensitat;
	return lightAmount;
}

vec4 PointCalculLightAmount(float diffuse, float inten, float specular, int i, vec3 normal, vec3 lightDir)
{
	vec3 fragToLight = crntPos - lights[i].lightPos;
	
	float closestDepth = texture(lights[i].depthCubeMap, fragToLight).r;
	closestDepth *= farPlane;
	float currentDepth = length(fragToLight);
	
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	vec4 lightAmount;
	lightAmount =  texture(diffuse0, texCoord) * (diffuse*inten*(1.0-shadow)+ambient) + inten * (1.0-shadow) * specular;
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
	float specular = specAmount * specularLight * 0.2;
	
	return PointCalculLightAmount(diffuse, inten, specular, i, normal, lightDirection);


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
	
	return DirecCalculLightAmount(diffuse, specular, i, normal, lightDirection);

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

	return PointCalculLightAmount(diffuse, inten, specular, i, normal, lightDirection);
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
	/*
	vec3 fragToLight = crntPos - lights[1].lightPos;
	float closestDepth = texture(lights[1].depthCubeMap, fragToLight).r;
	FragColor = vec4(vec3(closestDepth), 1.0);	
	*/
}