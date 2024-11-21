#include "skybox.h"
#include "funcionsEscena.h"

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

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width, height;
	get_resolution(width, height);
	//Creem la finsestra i comprobem que no hi hagi hagut cap error
	GLFWwindow* window = glfwCreateWindow(width, height, "Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}
	glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);

	//Creem finestra de visualització
	gladLoadGL();

	glViewport(0, 0, width, height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//------------------------------// esto no se porque influye en una cara del skybox que le da la vuelta si lo borro ademas influye en el color de algun objeto no entiendo nada;

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planks.png", "specular",1, GL_RED, GL_UNSIGNED_BYTE)
	};
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	//------------------------------//

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
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 10.0f, 5.0f);

	SetupModels(models, modelMatrices);

	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 12.0f));
	glm::vec3 camPos = camera.Position;

	glm::vec3 posLlum1 = glm::vec3(3, 3, 0);
	glm::mat4 modelLlum1 = glm::mat4(1.0f);
	modelLlum1 = glm::translate(modelLlum1, posLlum1);

	glm::vec3 posLlum2 = glm::vec3(0, 3, 3);
	glm::mat4 modelLlum2 = glm::mat4(1.0f);
	modelLlum2 = glm::translate(modelLlum2, posLlum2);
	
	glm::vec3 posLlum3 = glm::vec3(-3, 3, 0);
	glm::mat4 modelLlum3 = glm::mat4(1.0f);
	modelLlum3 = glm::translate(modelLlum3, posLlum3);

	glm::vec4 color1 = glm::vec4(1, 1, 1, 1);
	glm::vec4 color2 = glm::vec4(1, 1, 1, 1);
	glm::vec4 color3 = glm::vec4(1, 1, 1, 1);

	std::vector<Llum> llums;
		
	llums.push_back({ true, posLlum1, color1, Punt, 1, &llum1, &modelLlum1});
	llums.push_back({ true, posLlum2, color2, Foco, 1, &llum2, &modelLlum2});
	llums.push_back({ true, posLlum3, color3, Punt, 1, &llum3, &modelLlum3 });

	camera.RotateCamera(-12.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	camera.RotateCamera(-7.0f, glm::vec3(0.0f, 1.0f, 0.0f));


	//objetosShader.Activate();


	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	//Activem el depth test perque les coses mes llunyanes no es dibuixin sobre les properes
	glEnable(GL_DEPTH_TEST);

	//Instànciem una skybox
	Skybox skybox;
	//Inicialitzem l'Skybox
	skybox.initSkybox(skyboxShader);


	//Bucle principal
	while (!glfwWindowShouldClose(window)) {
		float i = glfwGetTime();
		//Definim color de fons
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Pintem color de fons
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//S'encarrega de tots els inputs de la camera
		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		// Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		glDepthFunc(GL_LEQUAL);

		DrawModels(shaderProgram, models, modelMatrices, llums, camera);
		
		DrawLights(lightShader, llums, camera);
		//Mostrem l'skybox.
		skybox.drawSkybox(skyboxShader, camera);

		//Cambiem el buffer que esta en pantalla pel que acabem de dibuixar en aquesta iteració
		glfwSwapBuffers(window);

		//Llegim tots els events glfw, ja sigui tancar la finestra o llegir inputs del teclat/ratolí/gamepad
		glfwPollEvents();
		glFinish();
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		std::cout << 1 / (glfwGetTime() - i) << std::endl;
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