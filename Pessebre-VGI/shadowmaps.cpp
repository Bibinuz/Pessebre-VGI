#include "shadowmaps.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int BASE_SHADOW_UNIT = 22; //22 23 24 25 26 27 28 29 30 31 Posicions per a tots els depth maps -> 10 llums maxim, 10 depth maps diferents
void InicialitzarShadowMap(std::vector<Llum>& llums)
{
	for (size_t i = 0; i < llums.size(); i++) {
		unsigned int depthMapFBO, depthMap;

		glGenFramebuffers(1, &depthMapFBO);

		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		llums[i].shadowUnit = BASE_SHADOW_UNIT + i;
		llums[i].depthMap = depthMap;
		llums[i].depthMapFBO = depthMapFBO;

	}

}
void RenderitzarShadowMap(std::vector<Llum>& llums, Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glCullFace(GL_FRONT);
	for (size_t i = 0; i < llums.size(); i++)
	{
		if (llums[i].sw_light)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, llums[i].depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glm::mat4 lightSpaceMatrix;
			glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
			glm::mat4 lightView = glm::lookAt(llums[i].lightPos,//Aqui va la posicio de la camera
				glm::vec3(0.0f, 0.0f, 0.0f), //Aqui va cap a on mira la camera
				glm::vec3(0.0f, 1.0f, 0.0f));//Direccio amunt

			lightSpaceMatrix = lightProjection * lightView;


			DrawDepthMap(shader, models, modelMatrices, llums[i].lightPos, lightSpaceMatrix);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			llums[i].lightSpaceMatrix = lightSpaceMatrix;
		}
	}
	//glCullFace(GL_BACK);
}
