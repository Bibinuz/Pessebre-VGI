#include "Shadowmap.h"

Shadowmap::Shadowmap()
{
	unit = 0;
	resolution = 0;
	depthMap = 0;
	depthMapFBO = 0;
	lightSpaceMatrix = glm::mat4(1.0);
}

Shadowmap::Shadowmap(short res, unsigned int u)
{
	unit = BASE_SHADOW_UNIT + u;
	resolution = 1 << res;
	lightSpaceMatrix = glm::mat4(1.0);

	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Shadowmap::RenderitzarShadowMap(glm::vec3& lightPos, Shader& shader,const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices)
{
	glViewport(0, 0, resolution, resolution);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	glm::mat4 lightView = glm::lookAt(lightPos,//Aqui va la posicio de la camera
	glm::vec3(0.0f, 0.0f, 0.0f), //Aqui va cap a on mira la camera
	glm::vec3(0.0f, 1.0f, 0.0f));//Direccio amunt
	lightSpaceMatrix = lightProjection * lightView;
	DrawDepthMap(shader, models, modelMatrices, lightPos);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadowmap::DrawDepthMap(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, glm::vec3& lightPos) {
	shader.Activate();
	glUniform3fv(glGetUniformLocation(shader.ID, "aPos"), 1, glm::value_ptr(lightPos));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	for (size_t i = 0; i < models.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		models[i].objecte->VAO.Bind();
		glDrawElements(GL_TRIANGLES, models[i].objecte->indices.size(), GL_UNSIGNED_INT, 0);
	}
}