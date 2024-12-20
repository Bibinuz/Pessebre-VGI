#pragma once
#include "skybox.h"
#include "funcionsEscena.h"
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
#include "imGuiImplementation.h"
#include "Shadowmap.h"

#include <irrKlang.h> // A�adido para el manejo de m�sica de fondo


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
	//Creem la finsestra i comprobem que no hi hagi hagut cap error
	GLFWwindow* window = glfwCreateWindow(width, height, "Pessebre VGI", NULL, NULL);
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




	// Aquesta part no hem funciona----------------------------------------------------------------

	// Crear el motor de sonido
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	if (!engine) {
		std::cout << "Could not startup engine" << std::endl;
		return 0;
	}

	// Ajusta esta ruta absoluta al archivo de audio (mp3, wav u ogg) que quieras probar
	const char* musicPath = "audio/musicaNadal.wav";
	irrklang::ISound* music = engine->play2D(musicPath, true);

	//engine->setSoundVolume(1.0f);
	//ISound* music = engine->play2D(musicPath, true);
	if (!music) {
		std::cout << "Failed to load music file. Check the path: " << musicPath << std::endl;
	}
	else {
		std::cout << "Music is playing." << std::endl;
	}

	//---------------------------------------------------------------------------------------------


	// Limites de posici�n para cada c�mara (ejemplo)
	glm::vec3 minPos1(-30.0f, 2.0f, -30.0f);
	glm::vec3 maxPos1(20.0f, 20.0f, 20.0f);

	// Definim c�meres
	// Definim c�meres
	Camera camBackground(width, height, glm::vec3(0.0f, 4.0f, 18.00f), minPos1, maxPos1);
	camBackground.cameraActive = false;

	Camera cameraEstatica(width, height, glm::vec3(11.0f, 6.2f, -30.0f), minPos1, maxPos1); // C�mara est�tica con l�mites
	cameraEstatica.Orientation = glm::vec3(-0.47f, -0.01f, 0.88f);

	cameraEstatica.cameraActive = false;

	glm::vec3 minPos2(10.0f, 6.0f, 10.0f);
	glm::vec3 maxPos2(10.0f, 6.0f, 10.0f);


	Camera camera1(width, height, glm::vec3(11.0f, 6.2f, -30.0f), minPos1, maxPos1); // Camera principal
	camera1.Orientation = glm::vec3((float)(-0.47), (float)(0), (float)(0.88));
	camera1.cameraActive = false;

	Camera camera2(width, height, glm::vec3(0.3f, 4.4f, -8.9f), minPos1, maxPos1); // camera bb
	camera2.Orientation = glm::vec3((float)(0), (float)(-0.3), (float)(0.95));
	camera2.cameraActive = false;


	Camera camera3(width, height, glm::vec3(8.9f, 20.0f, -27.7f), minPos1, maxPos1); // camera aerea
	camera3.Orientation = glm::vec3((float)(-0.3), (float)(-0.6), (float)(0.74));
	camera3.cameraActive = false;

	std::vector<Camera> Cameres;
	Cameres.push_back(camera1); Cameres.push_back(camera2); Cameres.push_back(camera3);
	Camera* camera = &camera1;

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

	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh llum1(lightVerts, lightInd, tex);

	std::vector<Model> models;
	std::vector<glm::mat4> modelMatrices;
	glm::vec4 lightColor = glm::vec4(1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 10.0f, 5.0f);

	SetupModels(models, modelMatrices);

	glm::vec3 camPos = camera->Position;

	glm::vec3 posLlum1 = glm::vec3(1.2f, 3.2f, -11.1f);
	glm::mat4 modelLlum1 = glm::mat4(1.0f);
	modelLlum1 = glm::translate(modelLlum1, posLlum1);


	glm::vec3 posLlum2 = glm::vec3(7.0f, 18.0f, 9.0f);
	glm::mat4 modelLlum2 = glm::mat4(1.0f);
	modelLlum2 = glm::translate(modelLlum2, posLlum2);

	glm::vec3 posLlum3 = glm::vec3(0.2f, 5.0f, -2.8f);
	glm::mat4 modelLlum3 = glm::mat4(1.0f);
	modelLlum3 = glm::translate(modelLlum3, posLlum3);

	glm::vec4 color1 = glm::vec4(1);
	glm::vec4 color2 = glm::vec4(1);
	glm::vec4 color3 = glm::vec4(1);

	std::vector<Llum> ls;
	std::vector<Llum> llums;

	ls.push_back({ true, posLlum1, glm::vec4(1.0f, 0.639f, 0.329f, 1.0f), Punt, 3, &llum1, modelLlum1 });
	ls.push_back({ true, posLlum2, color2, Direccional, 1.5, &llum1, modelLlum2 });
	ls.push_back({ true, posLlum3, color3, Foco, 2, &llum1, modelLlum3 });

	/* Shadow maps */
	Shader depthShader("depth.vert", "depth.frag");

	int diff = 0;
	int resolutionShadowMap = 11;
	for (int i = 0; i - diff < ls.size(); i++) {
		if (ls[i - diff].tipus == Direccional)
		{
			ls[i - diff].shadowmap = Shadowmap(resolutionShadowMap, i);
			ls[i - diff].shadowmap.lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.5f, 500.0f);
			llums.push_back(ls[i - diff]);
		}
		else if (ls[i - diff].tipus == Punt)
		{
			Llum aux[6];
			for (int j = 0; j < 6; j++)
			{
				aux[j] = { ls[i - diff].sw_light, ls[i - diff].lightPos, ls[i - diff].lightCol, Punt, ls[i - diff].intensitat, ls[i - diff].mesh, ls[i - diff].model };
				aux[j].shadowmap = Shadowmap(resolutionShadowMap, i + j);
				aux[j].shadowmap.lightProj = glm::perspective(90.0f, 1.0f, 0.5f, 500.0f);
				llums.push_back(aux[j]);
			}
			llums[i + 0].shadowmap.lightDir = glm::vec3(1.0, 0.0, 0.0);
			llums[i + 1].shadowmap.lightDir = glm::vec3(-1.0, 0.0, 0.0);
			llums[i + 2].shadowmap.lightDir = glm::vec3(0.0, 1.0, 0.0);
			llums[i + 3].shadowmap.lightDir = glm::vec3(0.0, -1.0, 0.0);
			llums[i + 4].shadowmap.lightDir = glm::vec3(0.0, 0.0, 1.0);
			llums[i + 5].shadowmap.lightDir = glm::vec3(0.0, 0.0, -1.0);

			llums[i + 0].shadowmap.lightUp = glm::vec3(0.0, -1.0, 0.0);
			llums[i + 1].shadowmap.lightUp = glm::vec3(0.0, -1.0, 0.0);
			llums[i + 2].shadowmap.lightUp = glm::vec3(0.0, 0.0, 1.0);
			llums[i + 3].shadowmap.lightUp = glm::vec3(0.0, 0.0, -1.0);
			llums[i + 4].shadowmap.lightUp = glm::vec3(0.0, -1.0, 0.0);
			llums[i + 5].shadowmap.lightUp = glm::vec3(0.0, -1.0, 0.0);
			i += 5;
			diff += 5;
		}
		else if (ls[i - diff].tipus == Foco)
		{
			ls[i - diff].shadowmap = Shadowmap(resolutionShadowMap, i);
			//ls[i - diff].shadowmap.lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.5f, 100.0f);
			ls[i - diff].shadowmap.lightProj = glm::perspective(90.0f, 1.0f, 0.5f, 500.0f);
			ls[i - diff].shadowmap.lightDir = glm::vec3(0.0, -1.0, 0.0);
			ls[i - diff].shadowmap.lightUp = glm::vec3(0.0, 0.0, -1.0);
			llums.push_back(ls[i - diff]);
		}
	}


	//Inst�nciem una skybox
	Skybox skybox;
	skybox.initSkybox(skyboxShader);

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// Configuraci� d'ImGui
	imGuiImplementation varImgui(window);

	int windowWidth, windowHeight;
	int llumAControlar = 0;
	int numLlumsAControlar = 6;



	// Bucle principal
	while (!glfwWindowShouldClose(window) && varImgui.op != Exit) {
		// Calculem el temps per al frame rate
		float i = glfwGetTime();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthFunc(GL_LEQUAL);

		for (int i = 0; i < llums.size(); i++)
		{
			llums[i].shadowmap.RenderitzarShadowMap(llums[i].lightPos, depthShader, models, modelMatrices);
		}
		glViewport(0, 0, width, height);

		// Inputs i actualitzaci� de la c�mera
		//std::cout << "Ha entrat a imput";
		if (varImgui.op != Menu && varImgui.op != StaticCamera)
			camera->Inputs(window);
		camera->UpdateMatrix(45.0f, 0.1f, 500.0f);
		//-----------------------------------------------
		varImgui.imGuiInitNewFrame();
		//-----------------------------------------------


		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

		DrawModels(shaderProgram, models, modelMatrices, llums, *camera);
		//DrawLights(lightShader, llums, *camera);
		skybox.drawSkybox(skyboxShader, *camera);

		switch (varImgui.op) {
		case Juga:
			//camera = &Cameres[0];

			ImGui::SetNextWindowPos(ImVec2(1650, 10));
			ImGui::Begin("Juga", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
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
			varImgui.imGuiLightsPos(llums[0].lightPos, llums[6].lightPos, llums[7].lightPos);
			break;

		case StaticCamera:
			varImgui.imGuiStaticCamera(camera, cameraEstatica);

			ImGui::SetNextWindowPos(ImVec2(1650, 10));
			ImGui::Begin("Static Camera Button", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

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
			break;

		case Credits:
			varImgui.imGuiCredits(windowWidth, windowHeight);
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

		// Actualitzem la mida de la finestra i el viewport
		//glfwGetWindowSize(window, &width, &height);
		//glViewport(0, 0, width, height);

		// Imprimim el frame rate
		//std::cout << 1 / (glfwGetTime() - i) << std::endl;




		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			llumAControlar = 0;
			numLlumsAControlar = 6;
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			llumAControlar = 6;
			numLlumsAControlar = 1;
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			llumAControlar = 7;
			numLlumsAControlar = 1;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			for (int i = llumAControlar; i < llumAControlar + numLlumsAControlar; i++) {
				llums[i].lightPos.x += 0.1;
				llums[i].model = glm::translate(glm::mat4(1.0), llums[i].lightPos);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			for (int i = llumAControlar; i < llumAControlar + numLlumsAControlar; i++) {
				llums[i].lightPos.x -= 0.1;
				llums[i].model = glm::translate(glm::mat4(1.0), llums[i].lightPos);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			for (int i = llumAControlar; i < llumAControlar + numLlumsAControlar; i++) {
				llums[i].lightPos.z -= 0.1;
				llums[i].model = glm::translate(glm::mat4(1.0), llums[i].lightPos);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			for (int i = llumAControlar; i < llumAControlar + numLlumsAControlar; i++) {
				llums[i].lightPos.z += 0.1;
				llums[i].model = glm::translate(glm::mat4(1.0), llums[i].lightPos);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		{
			for (int i = llumAControlar; i < llumAControlar + numLlumsAControlar; i++) {
				llums[i].lightPos.y += 0.1;
				llums[i].model = glm::translate(glm::mat4(1.0), llums[i].lightPos);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		{
			for (int i = llumAControlar; i < llumAControlar + numLlumsAControlar; i++) {
				llums[i].lightPos.y -= 0.1;
				llums[i].model = glm::translate(glm::mat4(1.0), llums[i].lightPos);
			}
		}
	}

	shaderProgram.Delete();
	skyboxShader.Delete();
	skybox.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	// Liberar el motor de sonido al terminar
	//engine->drop();

	return 0;
}
