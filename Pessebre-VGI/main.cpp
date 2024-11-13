#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imGuiImplementation.h"

#include"Model.h"

void get_resolution(int& width, int& height) {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = mode->width;
	height = mode->height;
}

Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), /*glm::vec3(0.0f, 1.0f, 0.0f),*/ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f),/* glm::vec3(0.0f, 1.0f, 0.0f),*/ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), /*glm::vec3(0.0f, 1.0f, 0.0f), */glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), /*glm::vec3(0.0f, 1.0f, 0.0f), */glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

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

	//------------------------------//

	Texture textures[]
	{
		Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("planks.png", "specular",1, GL_RED, GL_UNSIGNED_BYTE)
	};


	//------------------------------//

	Shader shaderProgram("default.vert", "default.frag");
	Shader treeShader("default.vert", "default.frag");
	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);

	Model proba;
	proba.loadObj("models3d/tree2.obj");
	proba.objecte->textures = tex;

	//------------------------------//
	Shader lightShader("light.vert", "light.frag");

	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex);

	//------------------------------//
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(10.5f, 0.5f, 10.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	//------------------------------//
	glm::vec3 object1Pos = glm::vec3(10.0f, 0.0f, 10.0f);
	glm::mat4 object1Model = glm::mat4(1.0f);
	object1Model = glm::translate(object1Model, object1Pos);
	//------------------------------//
	glm::vec3 treePos = glm::vec3(0, 0, 0);
	glm::mat4 treeModel = glm::mat4(1.0f);
	treeModel = glm::translate(treeModel, treePos);

	treeShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(treeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(treeModel));
	glUniform4f(glGetUniformLocation(treeShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(treeShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(object1Model));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//Activem el depth test perque les coses mes llunyanes no es dibuixin sobre les properes
	glEnable(GL_DEPTH_TEST);

	//Camera camera(width, height, glm::vec3(0.0f, 6.0f, 0.0f));


	// Definim tres càmeres amb diferents posicions
	Camera camera1(width, height, glm::vec3(0.0f, 6.0f, 0.0f)); // Càmera inicial
	Camera camera2(width, height, glm::vec3(10.0f, 6.0f, 10.0f)); // Segona càmera
	Camera camera3(width, height, glm::vec3(-10.0f, 6.0f, -10.0f)); // Tercera càmera
	std::vector<Camera> Cameres;
	Cameres.push_back(camera1); Cameres.push_back(camera2); Cameres.push_back(camera3);
	Camera* camera = &camera1; // Inicialitzem la càmera activa

	// Configuració d'ImGui
	imGuiImplementation varImgui(window);

	// Bucle principal
	while (!glfwWindowShouldClose(window)) {
		// Calculem el temps per al frame rate
		float i = glfwGetTime();

		// Esborrem el color de fons
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Inputs i actualització de la càmera
		camera->Inputs(window);
		camera->UpdateMatrix(45.0f, 0.1f, 100.0f);

		// Renderitzem l'escena amb la càmera activa
		proba.objecte->Draw(treeShader, *camera);
		floor.Draw(shaderProgram, *camera);
		light.Draw(lightShader, *camera);

		// Iniciem el nou frame d'ImGui
		varImgui.imGuiInitNewFrame();

		//Selector entre les tres cameres
		varImgui.cameraSelector(Cameres, camera);


		// Renderització d'ImGui
		varImgui.imGuiRender();

		// Intercanviem el buffer de la pantalla
		glfwSwapBuffers(window);

		// Processa els esdeveniments
		glfwPollEvents();
		glFinish();

		// Actualitzem la mida de la finestra i el viewport
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Imprimim el frame rate
		std::cout << 1 / (glfwGetTime() - i) << std::endl;
	}

	//Netegem tot el que hem utilitzat
	shaderProgram.Delete();
	lightShader.Delete();

	//Destruim la finestra per alliberar la memoria
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
