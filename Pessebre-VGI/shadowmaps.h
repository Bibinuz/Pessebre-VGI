#pragma once

#include "funcionsEscena.h"

void InicialitzarShadowMap(std::vector<Llum>& llums);

void RenderitzarShadowMap(std::vector<Llum>& llums, Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices);
