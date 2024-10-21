#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
	vec3 lightVec = lightPos-crntPos;
	float dist = length(lightVec);
	float a = 0.01;
	float b = 0.01;
	float c = 1;
	float inten = 1.0f/(a*dist*dist + b*dist + c);

	float ambient = 0.20f;
	
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - crntPos);	
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return texture(diffuse0, texCoord) * ((diffuse*inten+ambient) + texture(specular0, texCoord).r * specular *  inten) * lightColor;

}

vec4 direcLight()
{
	
	float ambient = 0.20f;

	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));	
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	return texture(diffuse0, texCoord) * ((diffuse+ambient) + texture(specular0, texCoord).r * specular) * lightColor;

}

vec4  spotLight(){

	float inner_cone = 0.95f;
	float outer_cone = 0.9f;

	float ambient = 0.20f;
	
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - crntPos);	
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten =  clamp((angle-outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f); 

	return texture(diffuse0, texCoord) * ((diffuse*inten+ambient) + texture(specular0, texCoord).r * specular  * inten) * lightColor;
}

void main()
{
	
	FragColor = pointLight();
}