#pragma once

#include "Model.h"
#include "Shadowmap.h"

enum TipusLlum
{
	Punt,
	Foco,
	Direccional
};

struct Llum {
	bool sw_light;
	glm::vec3 lightPos;
	glm::vec4 lightCol;
	TipusLlum tipus;
	float intensitat;
	Mesh* mesh;
	glm::mat4 model;
	Shadowmap shadowmap;
};

void get_resolution(int& width, int& height);

void CreateModel(Model& model, const std::string& filePath, const glm::vec3& position, const glm::vec3& scale, glm::mat4& modelMatrix, float rotationAngle = 0.0f, const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f));

void BindTextures(Shader& shader, const Model& model);

void DrawModelWithShader(Shader& shader, const Model& model, const glm::mat4& modelMatrix, Camera& camera);

void SetupModels(std::vector<Model>& models, std::vector<glm::mat4>& modelMatrices);

void DrawModels(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, std::vector<Llum>& llums, Camera& camera);

void DrawLights(Shader& shader, std::vector<Llum>& llums, Camera& camera);

void DrawDepthMap(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, glm::vec3& lightPos, glm::mat4& lightSpaceMatrix);