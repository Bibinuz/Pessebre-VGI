#pragma once
#include "skybox.h"
#include "funcionsEscena.h"
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
#include "imGuiImplementation.h"

int TOTAL_CAGANERS = 5;

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

GLFWwindow* inicialitzaFinestra() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width, height;
	get_resolution(width, height);
	//Creem la finestra
	GLFWwindow* window = glfwCreateWindow(width, height, "Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return window;
	}
	glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);

	//Creem context OpenGL
	gladLoadGL();
	glViewport(0, 0, width, height);

	//Activem el depth test
	glEnable(GL_DEPTH_TEST); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return window;
}

int main() {

	GLFWwindow* window = inicialitzaFinestra();

	int width, height;
	get_resolution(width, height);

	// Definim c�meres
	Camera camBackground(width, height, glm::vec3(0.0f, 4.0f, 18.00f));
	camBackground.cameraActive = false;
	Camera cameraEstatica(width, height, glm::vec3(0.0f, 4.0f, 18.00f));
	cameraEstatica.cameraActive = false;
	cameraEstatica.cameraEstatica = true;
	Camera camera1(width, height, glm::vec3(0.0f, 6.0f, 0.0f));
	Camera camera2(width, height, glm::vec3(10.0f, 6.0f, 10.0f));
	Camera camera3(width, height, glm::vec3(-10.0f, 6.0f, -10.0f));
	std::vector<Camera> Cameres;
	Cameres.push_back(camera1); Cameres.push_back(camera2); Cameres.push_back(camera3);
	Camera* camera = &cameraEstatica;

	Texture textures[]{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planks.png", "specular",1, GL_RED, GL_UNSIGNED_BYTE)
	};
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	Shader shaderProgram("default.vert", "default.frag");
	Shader lightShader("light.vert", "light.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh llum1(lightVerts, lightInd, tex);
	Mesh llum2(lightVerts, lightInd, tex);
	Mesh llum3(lightVerts, lightInd, tex);

	std::vector<Model> models;
	std::vector<glm::mat4> modelMatrices;
	glm::vec4 lightColor = glm::vec4(1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 10.0f, 5.0f);

	SetupModels(models, modelMatrices);

	glm::vec3 posLlum1 = glm::vec3(3, 3, 0);
	glm::mat4 modelLlum1 = glm::translate(glm::mat4(1.0f), posLlum1);

	glm::vec3 posLlum2 = glm::vec3(0, 3, 3);
	glm::mat4 modelLlum2 = glm::translate(glm::mat4(1.0f), posLlum2);

	glm::vec3 posLlum3 = glm::vec3(-3, 3, 0);
	glm::mat4 modelLlum3 = glm::translate(glm::mat4(1.0f), posLlum3);

	glm::vec4 color1 = glm::vec4(1);
	glm::vec4 color2 = glm::vec4(1);
	glm::vec4 color3 = glm::vec4(1);

	std::vector<Llum> llums;
	llums.push_back({ true, posLlum1, color1, Punt, 1, &llum1, &modelLlum1 });
	llums.push_back({ true, posLlum2, color2, Foco, 1, &llum2, &modelLlum2 });
	llums.push_back({ true, posLlum3, color3, Punt, 1, &llum3, &modelLlum3 });

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	Skybox skybox;
	skybox.initSkybox(skyboxShader);

	imGuiImplementation varImgui(window);

	int windowWidth, windowHeight;
	int caganers = 0;

	while (!glfwWindowShouldClose(window) && varImgui.op != Exit) {
		float i = glfwGetTime();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthFunc(GL_LEQUAL);
		camera->Inputs(window);
		camera->UpdateMatrix(45.0f, 0.1f, 100.0f);

		varImgui.imGuiInitNewFrame();
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

		DrawModels(shaderProgram, models, modelMatrices, llums, *camera);
		DrawLights(lightShader, llums, *camera);
		skybox.drawSkybox(skyboxShader, *camera);

		switch (varImgui.op) {
		case Juga:
			// C�mera
			camera = &Cameres[0];

			ImGui::SetNextWindowPos(ImVec2(1650, 10));
			ImGui::Begin("Juga", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

			// Fem el mateix estil que als altres botons
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

			// Bot� "Tornar al menu" amb imatge
			if (ImGui::ImageButton("return_btn_juga", (ImTextureID)(uintptr_t)varImgui.buttonReturnTextureID, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1))) {
				varImgui.op = Menu;
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);
			ImGui::End();
			break;

		case Manager:
			varImgui.cameraSelector(Cameres, camera);

			ImGui::SetNextWindowPos(ImVec2(1650, 10));
			ImGui::Begin("Manager", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

			if (ImGui::ImageButton("return_btn_manager", (ImTextureID)(uintptr_t)varImgui.buttonReturnTextureID, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1))) {
				varImgui.op = Menu;
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);
			ImGui::End();

			varImgui.imGuiShowFPS();
			varImgui.imGuiCamPosition(camera);
			break;

		case StaticCamera:
			varImgui.imGuiStaticCamera(camera, cameraEstatica);

			ImGui::Begin("Static Camera");

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

			if (ImGui::ImageButton("return_btn_static", (ImTextureID)(uintptr_t)varImgui.buttonReturnTextureID, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1))) {
				varImgui.op = Menu;
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);
			ImGui::End();
			break;

		case Controls:
			varImgui.imGuiControls(windowWidth, windowHeight);
			// Ja no posem el segon bot� textual aqu�, 
			// perqu� imGuiControls() ja inclou el bot� d'imatge "Tornar al menu"
			break;

		case Credits:
			varImgui.imGuiCredits(windowWidth, windowHeight);
			// Igual que a Controls, no posem un segon bot�, 
			// ja que imGuiCredits() ja t� el bot� d'imatge
			break;

		case Menu:
			camera = &camBackground;
			varImgui.imGuiMainMenu(windowWidth, windowHeight, camera);
			break;

		case Exit:
			camera = &camBackground;
			varImgui.imGuiMainMenu(windowWidth, windowHeight, camera);
			break;

		default:
			std::cout << "Esperant selecci�..." << std::endl;
			break;
		}

		varImgui.imGuiRender();
		glfwSwapBuffers(window);
		glfwPollEvents();
		glFinish();

		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	shaderProgram.Delete();
	skyboxShader.Delete();
	skybox.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
