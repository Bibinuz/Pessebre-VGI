#pragma once
#include "skybox.h"
#include "funcionsEscena.h"
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
#include "imGuiImplementation.h"
#include "Shadowmap.h"
#include "CubShadowMap.h"

#define print std::cout<<

int TOTAL_CAGANERS=5;

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

static GLFWwindow* inicialitzaFinestra() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width, height;
	get_resolution(width, height);
	//Creem la finsestra i comprobem que no hi hagi hagut cap error
	GLFWwindow* window = glfwCreateWindow(width, height, "Pessebre VGI", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return window;
	}
	glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);

	//Creem finestra de visualitzaci�
	gladLoadGL();

	glViewport(0, 0, width, height);

	//Activem el depth test perque les coses mes llunyanes no es dibuixin sobre les properes
	glEnable(GL_DEPTH_TEST); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return window;
}

int main() {

	GLFWwindow* window = inicialitzaFinestra();

	int width, height;
	get_resolution(width, height);

	// Definim tres c�meres amb diferents posicions
	Camera cameraEstatica(width, height, glm::vec3(0.0f, 6.0f, 0.0f)); // C�mera inicial
	cameraEstatica.cameraActive = false;
	Camera camera2(width, height, glm::vec3(10.0f, 6.0f, 10.0f)); // Segona c�mera
	Camera camera3(width, height, glm::vec3(-10.0f, 6.0f, -10.0f)); // Tercera c�mera
	std::vector<Camera> Cameres;
	Cameres.push_back(cameraEstatica); Cameres.push_back(camera2); Cameres.push_back(camera3);
	Camera* camera = &cameraEstatica; // Inicialitzem la c�mera activa




	//------------------------------// esto no se porque influye en una cara del skybox que le da la vuelta si lo borro ademas influye en el color de algun objeto no entiendo nada;

	Texture textures[2]
	{
		Texture("", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("", "specular",0, GL_RED, GL_UNSIGNED_BYTE)
	};


	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	//------------------------------//

	Shader shaderProgram("default.vert", "default.frag");
	Shader lightShader("light.vert", "light.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");
	Shader depthShader("depth.vert", "depth.frag");
	Shader depthCubeShader("depthCube.vert", "depthCube.geom", "depthCube.frag");

	
	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh llum1(lightVerts, lightInd, tex);
	Mesh llum2(lightVerts, lightInd, tex);
	Mesh llum3(lightVerts, lightInd, tex);



	std::vector<Model> models;
	std::vector<glm::mat4> modelMatrices;
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 10.0f, 5.0f);

	SetupModels(models, modelMatrices);

	glm::vec3 camPos = camera->Position;

	glm::vec3 posLlum1 = glm::vec3(0, 10, 10);
	glm::mat4 modelLlum1 = glm::mat4(1.0f);
	modelLlum1 = glm::translate(modelLlum1, posLlum1);


	glm::vec3 posLlum2 = glm::vec3(0.0, 0.05, 0.0);
	glm::mat4 modelLlum2 = glm::mat4(1.0f);
	modelLlum2 = glm::translate(modelLlum2, posLlum2);
	
	glm::vec3 posLlum3 = glm::vec3(5, 5, 0);
	glm::mat4 modelLlum3 = glm::mat4(1.0f);
	modelLlum3 = glm::translate(modelLlum3, posLlum3);

	glm::vec4 color1 = glm::vec4(1, 1, 1, 1);
	glm::vec4 color2 = glm::vec4(1, 1, 1, 1);
	glm::vec4 color3 = glm::vec4(1, 1, 1, 1);

	std::vector<Llum> llums;
		
	llums.push_back({ false, posLlum1, color1, Direccional, 1, &llum1, modelLlum1});
	llums.push_back({ true, posLlum2, color2, Punt, 1.5, &llum2, modelLlum2});
	llums.push_back({ false, posLlum3, color3, Direccional, 1, &llum3, modelLlum3 });

	/* Shadow maps */
	for (int i = 0; i < llums.size(); i++) {
		print "Afegint Llum: " << i << std::endl;
		if (llums[i].tipus == Direccional)
			llums[i].shadowmap = new Shadowmap(10, i);
		else if (llums[i].tipus == Punt)
			llums[i].shadowmap = new CubShadowMap(10, i);
	}
	
	print "Inicialitzant Skybox" << std::endl;

	//Inst�nciem una skybox
	Skybox skybox;
	//Inicialitzem l'Skybox
	skybox.initSkybox(skyboxShader);

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Configuraci� d'ImGui
	imGuiImplementation varImgui(window);

	int windowWidth, windowHeight;
	int caganers=0;

	// Bucle principal
	bool menysRes = true;
	bool mesRes = true;
	GLuint64 frameNum = 0;
	while (!glfwWindowShouldClose(window)&&varImgui.op!=Exit) {
		// Calculem el temps per al frame rate
		//float i = glfwGetTime();

		// Esborrem el color de fons
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		glDepthFunc(GL_LEQUAL);
		
		for (Llum& llum : llums)
		{
			if (llum.sw_light)
			{
				if (llum.tipus == Direccional) {
					llum.shadowmap->RenderitzarShadowMap(llum.lightPos, depthShader, models, modelMatrices);
				}
				else if (llum.tipus == Punt) {
					llum.shadowmap->RenderitzarShadowMap(llum.lightPos, depthCubeShader, models, modelMatrices);
				}
			}
		}
		glViewport(0, 0, width, height);
		// Inputs i actualitzaci� de la c�mera
		camera->Inputs(window);
		camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
		//-----------------------------------------------
		varImgui.imGuiInitNewFrame();
		//-----------------------------------------------


		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);		

		DrawModels(shaderProgram, models, modelMatrices, llums, *camera);
		
		DrawLights(lightShader, llums, *camera);

		skybox.drawSkybox(skyboxShader, *camera);

		switch (varImgui.op) {
		case Juga:
			//varImgui.cameraSelector(Cameres, camera);//------------SELECTOR DE CAMARES
			ImGui::SetNextWindowPos(ImVec2(1650, 10));

			ImGui::Begin("Juga",nullptr,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_AlwaysAutoResize);
			//ImGui::Text("%d/%d Caganers",caganers,TOTAL_CAGANERS); ImGui::SameLine();//----------------CAGANERS 
			//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//__________________________Per a sortir al menu principal amb ESC
			if (ImGui::Button("Tornar al menu")) {

				varImgui.op = Menu;  // Torna al men� principal
			}
			ImGui::End();
			break;

		case Manager:
			varImgui.cameraSelector(Cameres,camera);

			ImGui::SetNextWindowPos(ImVec2(1650, 10));
			ImGui::Begin("Manager", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::Button("Tornar al menu")) {
				varImgui.op = Menu;  // Torna al men� principal
			}
			ImGui::End();
			//Per mostrar fps
			varImgui.imGuiShowFPS();
			break;

		case StaticCamera:
			// Codi per activar una c�mera est�tica
			varImgui.cameraSelector(Cameres, camera);
			ImGui::Begin("Static Camera");
			if (ImGui::Button("Tornar al menu")) {
				varImgui.op = Menu;  // Torna al men� principal
			}
			ImGui::End();
			break;

		case Menu:
			varImgui.imGuiMainMenu(windowWidth, windowHeight);
			break;
		case Exit:
			varImgui.imGuiMainMenu(windowWidth, windowHeight);
			break;

			
		default:
			std::cout << "Esperant selecci�..." << std::endl;
			break;
		}

		varImgui.imGuiRender();
		glfwSwapBuffers(window);

		// Processa els esdeveniments
		glfwPollEvents();
		glFinish();

		// Actualitzem la mida de la finestra i el viewport
		//glfwGetWindowSize(window, &width, &height);
		//glViewport(0, 0, width, height);

		// Imprimim el frame rate
		//std::cout << 1 / (glfwGetTime() - i) << std::endl;

		int controlLlum = 1;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			llums[controlLlum].lightPos.x += 0.01;
			llums[controlLlum].model = glm::translate(glm::mat4(1.0), llums[controlLlum].lightPos);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			llums[controlLlum].lightPos.x -= 0.01;
			llums[controlLlum].model = glm::translate(glm::mat4(1.0), llums[controlLlum].lightPos);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{ 
			llums[controlLlum].lightPos.z += 0.01;
			llums[controlLlum].model = glm::translate(glm::mat4(1.0), llums[controlLlum].lightPos);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			llums[controlLlum].lightPos.z -= 0.01;
			llums[controlLlum].model = glm::translate(glm::mat4(1.0), llums[controlLlum].lightPos);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		{
			llums[controlLlum].lightPos.y += 0.01;
			llums[controlLlum].model = glm::translate(glm::mat4(1.0), llums[controlLlum].lightPos);
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{ 
			llums[controlLlum].lightPos.y -= 0.01;
			llums[controlLlum].model = glm::translate(glm::mat4(1.0), llums[controlLlum].lightPos);
		}

		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && menysRes)
		{
			menysRes = false;
			for (Llum& llum : llums)
				llum.shadowmap = new Shadowmap(llum.shadowmap->escalaRes - 1, llum.shadowmap->unit-22);
		}
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS&& mesRes)
		{
			mesRes = false;
			for (Llum& llum : llums)
				llum.shadowmap = new Shadowmap(llum.shadowmap->escalaRes + 1, llum.shadowmap->unit - 22);
			
		}
		if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_RELEASE)
		{
			mesRes = true;
		}
		if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_RELEASE)
		{
			menysRes = true;
		}
	}

	//Netegem tot el que hem utilitzat
	shaderProgram.Delete();
	//objetosShader.Delete();
	skyboxShader.Delete();

	//esborrem l'skybox.
	skybox.cleanup();

	//Destruim la finestra per alliberar la memoria
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}