#include "CubShadowMap.h"

CubShadowMap::CubShadowMap(){
	shadowProj = glm::mat4(1);
	farPlane = 100.0f;

}


CubShadowMap::CubShadowMap(short escala, unsigned int u)
{
	escalaRes = escala;
	resolution = 1 << escalaRes;
	unit = BASE_SHADOW_UNIT + u;
	lightSpaceMatrix = glm::mat4(1.0);
	shadowProj = glm::mat4(1.0);
	farPlane = 25.0f;
	for (int i = 0; i < 6; i++)
		shadowTransforms[i] = glm::mat4(1.0);


	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,	GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);\

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubShadowMap::RenderitzarShadowMap(glm::vec3& lightPos, Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices)
{
	glViewport(0, 0, resolution, resolution);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	CubShadowMap::ConfigurarMatrius(lightPos);
	CubShadowMap::DrawDepthMap(shader, models, modelMatrices, lightPos);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void CubShadowMap::ConfigurarMatrius(glm::vec3& lightPos)
{
	shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);

	shadowTransforms[0] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
	shadowTransforms[1] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
	shadowTransforms[2] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	shadowTransforms[3] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	shadowTransforms[4] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	shadowTransforms[5] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

}

void CubShadowMap::DrawDepthMap(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, glm::vec3& lightPos)
{
	shader.Activate();
	//glUniform3f(glGetUniformLocation(shader.ID, "aPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shader.ID, "far_plane"), farPlane);


	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));


	for (size_t i = 0; i < models.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		models[i].objecte->VAO.Bind();
		glDrawElements(GL_TRIANGLES, models[i].objecte->indices.size(), GL_UNSIGNED_INT, 0);
	}

}
