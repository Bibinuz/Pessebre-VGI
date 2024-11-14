#include"Model.h"

void get_resolution(int& width, int& height) {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = mode->width;
	height = mode->height;
}

Vertex vertices[] =
{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f),	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f),	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
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
	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex);

	//Shader treeShader("default.vert", "default.frag");
	//Model proba;
	//proba.loadObj("models3d/tree2.obj");
	//proba.objecte->textures = tex;

	//------------------------------//
	Shader llum1Shader("light.vert", "light.frag");
	Shader llum2Shader("light.vert", "light.frag");
	Shader llum3Shader("light.vert", "light.frag");

	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh llum1(lightVerts, lightInd, tex);
	Mesh llum2(lightVerts, lightInd, tex);
	Mesh llum3(lightVerts, lightInd, tex);

	//------------------------------//
	glm::vec4 colorLlum = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 posLlum1 = glm::vec3(1.5f, 1.5f, 1.5f);
	glm::mat4 modelLlum1 = glm::mat4(1.0f);
	modelLlum1 = glm::translate(modelLlum1, posLlum1);

	glm::vec3 posLlum2 = glm::vec3(-1.5f, 1.5f, -1.5f);
	glm::mat4 modelLlum2 = glm::mat4(1.0f);
	modelLlum2 = glm::translate(modelLlum2, posLlum2);

	glm::vec3 posLlum3 = glm::vec3(0.0f, 1.5f, 0.0f);
	glm::mat4 modelLlum3 = glm::mat4(1.0f);
	modelLlum3 = glm::translate(modelLlum3, posLlum3);
	//------------------------------//
	glm::vec3 object1Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 object1Model = glm::mat4(1.0f);
	object1Model = glm::scale(object1Model, glm::vec3(5, 1, 5));
	object1Model = glm::translate(object1Model, object1Pos);
	//------------------------------//
	//glm::vec3 treePos = glm::vec3(0, 0, 0);
	//glm::mat4 treeModel = glm::mat4(1.0f);
	//treeModel = glm::translate(treeModel, treePos);
	//
	//treeShader.Activate();
	//glUniformMatrix4fv(glGetUniformLocation(treeShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(treeModel));
	//glUniform4f(glGetUniformLocation(treeShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	//glUniform3f(glGetUniformLocation(treeShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	llum1Shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(llum1Shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelLlum1));
	glUniform4f(glGetUniformLocation(llum1Shader.ID, "lightColor"), colorLlum.x, colorLlum.y, colorLlum.z, colorLlum.w);
	llum2Shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(llum2Shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelLlum2));
	glUniform4f(glGetUniformLocation(llum2Shader.ID, "lightColor"), colorLlum.x, colorLlum.y, colorLlum.z, colorLlum.w);
	llum3Shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(llum2Shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelLlum3));
	glUniform4f(glGetUniformLocation(llum2Shader.ID, "lightColor"), colorLlum.x, colorLlum.y, colorLlum.z, colorLlum.w);


	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(object1Model));


	Llum l1 = { true, posLlum1, colorLlum, Punt, 1 };
	Llum l2 = { true, posLlum2, colorLlum, Foco, 1 };
	Llum l3 = { true, posLlum3, colorLlum, Foco, 3 };

	std::vector<Llum> llums;
	llums.push_back(l1);
	llums.push_back(l2);
	llums.push_back(l3);
	glUniform1i(glGetUniformLocation(shaderProgram.ID, "numLights"), llums.size());

	for (int i = 0; i < llums.size(); i++)
	{
		glUniform1i(glGetUniformLocation(shaderProgram.ID, ("lights[" + std::to_string(i) + "].sw_light").c_str()	),	llums[i].sw_light);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, ("lights[" + std::to_string(i) + "].lightPos").c_str()	),	llums[i].lightPos.x, llums[i].lightPos.y, llums[i].lightPos.z);
		glUniform4f(glGetUniformLocation(shaderProgram.ID, ("lights[" + std::to_string(i) + "].lightColor").c_str()	),	llums[i].lightCol.r, llums[i].lightCol.g, llums[i].lightCol.b, llums[i].lightCol.a);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, ("lights[" + std::to_string(i) + "].tipus").c_str()		),	llums[i].tipus);
		glUniform1f(glGetUniformLocation(shaderProgram.ID, ("lights[" + std::to_string(i) + "].intensitat").c_str()	),	llums[i].intensitat);
	}

	//Activem el depth test perque les coses mes llunyanes no es dibuixin sobre les properes
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 6.0f, 0.0f));

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

		//proba.objecte->Draw(treeShader, camera);
		floor.Draw(shaderProgram, camera);
		llum1.Draw(llum1Shader, camera);
		llum2.Draw(llum2Shader, camera);
		llum3.Draw(llum3Shader, camera);
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
	llum1Shader.Delete();

	//Destruim la finestra per alliberar la memoria
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
