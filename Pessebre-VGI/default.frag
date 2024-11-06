#version 330 core

struct Light{
	bool sw_light;
	vec3 lightPos;
	vec4 lightColor;
	int tipus;
	float intensitat;
};

out vec4 FragColor;

in vec3 crntPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec3 camPos;
uniform Light lights[10];
uniform int numLights;


vec4 pointLight(int i)
{ 
	float a = 0.01;
	float b = 0.01;
	float c = 1;
	float ambient = 0.20f;
	float specularLight = 0.50f;
		
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
	
	vec4 lightAmount = texture(diffuse0, texCoord) * ((diffuse*inten+ambient) + texture(specular0, texCoord).r * specular *  inten) * lights[i].lightColor;
	return lightAmount*lights[i].intensitat;
}

vec4 direcLight(int i)
{	
	float ambient = 0.20f;
	float specularLight = 0.50f;

	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));	
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	vec4 lightAmount = texture(diffuse0, texCoord) * ((diffuse+ambient) + texture(specular0, texCoord).r * specular) * lights[i].lightColor;
	return lightAmount*lights[i].intensitat;
}

vec4  spotLight(int i){

	float inner_cone = 0.95f;
	float outer_cone = 0.9f;
	float ambient = 0.20f;
	float specularLight = 0.50f;
	
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lights[i].lightPos - crntPos);	
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;
	
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten =  clamp((angle-outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f); 

	vec4 lightAmount = texture(diffuse0, texCoord) * ((diffuse*inten+ambient) + texture(specular0, texCoord).r * specular  * inten) * lights[i].lightColor;
	return lightAmount*lights[i].intensitat;
}

void main()
{
	FragColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < numLights; i++){
		switch(lights[i].tipus){
			
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