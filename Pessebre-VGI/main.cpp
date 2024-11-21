#include "Model.h"
#include "VAO.h"
#include "VBO.h"
#include "skybox.h"

void get_resolution(int& width, int& height) {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = mode->width;
	height = mode->height;
}

void CreateModel(Model& model, const std::string& filePath, const glm::vec3& position, const glm::vec3& scale, glm::mat4& modelMatrix, float rotationAngle = 0.0f, const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)) {
	model.loadObj(filePath.c_str());  // Cargar el modelo desde el archivo
	modelMatrix = glm::mat4(1.0f);  // Inicializar la matriz de transformación
	modelMatrix = glm::translate(modelMatrix, position);  // Aplicar la posición
	modelMatrix = glm::rotate(modelMatrix, rotationAngle, rotationAxis);  // Aplicar la rotación
	modelMatrix = glm::scale(modelMatrix, scale);  // Aplicar la escala
}

void BindTextures(Shader& shader, const Model& model) {
	// Comprobar si el modelo tiene texturas cargadas
	if (!model.textures.empty()) {
		shader.Activate();

		// Activar y vincular la textura
		glActiveTexture(GL_TEXTURE0);  // Usamos la primera unidad de textura
		glBindTexture(GL_TEXTURE_2D, model.textures[0].id);  // Vincular la primera textura del modelo

		// Asignar la textura al shader (suponiendo que en el shader se usa un sampler2D llamado "texture0")
		glUniform1i(glGetUniformLocation(shader.ID, "texture0"), 0);
	}
	else {
		std::cerr << "No se han cargado texturas para el modelo." << std::endl;
	}
}

void DrawModelWithShader(Shader& shader, const Model& model, const glm::mat4& modelMatrix, const glm::vec3& lightPos, const glm::vec4& lightColor, const glm::vec3& camPos, Camera& camera) {
	shader.Activate();

	// Configurar propiedades globales del shader
	glUniform3fv(glGetUniformLocation(shader.ID, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform4fv(glGetUniformLocation(shader.ID, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(shader.ID, "camPos"), 1, glm::value_ptr(camPos));

	// Configurar la matriz de modelo específica y dibujar
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	BindTextures(shader, model);  // Vincular texturas del modelo
	model.objecte->Draw(shader, camera);  // Dibujar el modelo
}

void SetupModels(std::vector<Model>& models, std::vector<glm::mat4>& modelMatrices) {
	// Lista de parámetros para cada modelo
	struct ModelData {
		std::string filePath;
		glm::vec3 position;
		glm::vec3 scale;
		float rotationAngle;  // Opcional: ángulo de rotación (en radianes)
		glm::vec3 rotationAxis;
	};

	std::vector<ModelData> modelDataList = {
		{ "models3d/superficie.obj", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/iglesia.obj", glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/dona1.obj", glm::vec3(1.0f, 0.05f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/moli.obj", glm::vec3(-5.0f, 0.05f, 0.0f), glm::vec3(0.75f, 0.75f, 0.75f),  0.0f, glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/gallina.obj", glm::vec3(-3.0f, 0.05f, 3.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::radians(140.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/cavall.obj", glm::vec3(3.0f, 0.05f, 3.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/arbre1.obj", glm::vec3(4.0f, 0.05f, 0.0f), glm::vec3(0.15f, 0.15f, 0.15f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f) }
	};

	// Crear cada modelo
	for (const auto& data : modelDataList) {
		models.emplace_back();  // Agregar un nuevo modelo
		modelMatrices.emplace_back();  // Agregar una nueva matriz de modelo
		CreateModel(models.back(), data.filePath, data.position, data.scale, modelMatrices.back(), data.rotationAngle, data.rotationAxis);
	}
}

void DrawModels(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, const glm::vec3& lightPos, const glm::vec4& lightColor, const glm::vec3& camPos, Camera& camera) {
	for (size_t i = 0; i < models.size(); ++i) {
		DrawModelWithShader(shader, models[i], modelMatrices[i], lightPos, lightColor, camPos, camera);
	}
}

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


	//------------------------------//

	Shader shaderProgram("default.vert", "default.frag");
	Shader objetosShader("object.vert", "object.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	std::vector<Model> models;
	std::vector<glm::mat4> modelMatrices;
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-2.0f, 10.0f, 5.0f);

	SetupModels(models, modelMatrices);

	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 12.0f));
	glm::vec3 camPos = camera.Position;

	//camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

	camera.RotateCamera(-12.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	camera.RotateCamera(-7.0f, glm::vec3(0.0f, 1.0f, 0.0f));


	objetosShader.Activate();


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

		DrawModels(objetosShader, models, modelMatrices, lightPos, lightColor, camPos, camera);

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
	objetosShader.Delete();
	skyboxShader.Delete();

	//esborrem l'skybox.
	skybox.cleanup();

	//Destruim la finestra per alliberar la memoria
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}