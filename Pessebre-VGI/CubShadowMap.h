#ifndef CUBSHADOWMAP_CLASS_H
#define CUBSHADOWMAP_CLASS_H

#pragma once
#include <string>
#include "Shadowmap.h"
class CubShadowMap : public Shadowmap
{
public:
	glm::mat4 shadowTransforms[6];
	glm::mat4 shadowProj;
	float farPlane;
	CubShadowMap();
	CubShadowMap(short escala, unsigned int unit);
	virtual void quiSoc()
	{
		std::cout << "Soc cubeShadowMap" << std::endl;
	};
	virtual void RenderitzarShadowMap(glm::vec3& lightPos, Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices);
	void ConfigurarMatrius(glm::vec3& lightPos);
	virtual void DrawDepthMap(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, glm::vec3& lightPos);
};

#endif
