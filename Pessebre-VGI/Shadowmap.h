#pragma once
#ifndef SHADOWMAP_CLASS_H
#define SHADOWMAP_CLASS_H

#include"Model.h"

const unsigned int BASE_SHADOW_UNIT = 22; //22 23 24 25 26 27 28 29 30 31 Posicions per a tots els depth maps -> 10 llums maxim, 10 depth maps diferents

class Shadowmap
{
public:
	short resolution;
	unsigned int depthMap;
	unsigned int depthMapFBO;
	unsigned int unit;
	glm::mat4 lightSpaceMatrix;

	Shadowmap();

	Shadowmap(short res, unsigned int u);

	void RenderitzarShadowMap(glm::vec3& lightPos, Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices);

	void DrawDepthMap(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, glm::vec3& lightPos);
	
};

#endif // 
