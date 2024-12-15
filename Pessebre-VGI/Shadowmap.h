#pragma once
#ifndef SHADOWMAP_CLASS_H
#define SHADOWMAP_CLASS_H

#include"Model.h"

const unsigned int BASE_SHADOW_UNIT = 4; 

class Shadowmap
{
public:
	short resolution;
	unsigned int depthMap;
	unsigned int depthMapFBO;
	unsigned int unit;
	glm::mat4 lightProj;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightDir;
	glm::vec3 lightUp;


	Shadowmap();

	Shadowmap(short res, unsigned int u);

	void RenderitzarShadowMap(glm::vec3& lightPos, Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices);

	void DrawDepthMap(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, glm::vec3& lightPos);
	
};

#endif // 
