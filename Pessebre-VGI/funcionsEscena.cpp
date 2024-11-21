#include "funcionsEscena.h"

void get_resolution(int& width, int& height) {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = mode->width;
	height = mode->height;
}

void CreateModel(Model& model, const std::string& filePath, const glm::vec3& position, const glm::vec3& scale, glm::mat4& modelMatrix, float rotationAngle, const glm::vec3& rotationAxis) {
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

void DrawModelWithShader(Shader& shader, const Model& model, const glm::mat4& modelMatrix, std::vector<Llum>& llums, Camera& camera) {
	shader.Activate();

	// Configurar propiedades globales del shader
	glUniform1i(glGetUniformLocation(shader.ID, "numLights"), llums.size());

	for (int i = 0; i < llums.size(); i++)
	{
		glUniform1i(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].sw_light").c_str()), llums[i].sw_light);
		glUniform3f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].lightPos").c_str()), llums[i].lightPos.x, llums[i].lightPos.y, llums[i].lightPos.z);
		glUniform4f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].lightColor").c_str()), llums[i].lightCol.r, llums[i].lightCol.g, llums[i].lightCol.b, llums[i].lightCol.a);
		glUniform1i(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].tipus").c_str()), llums[i].tipus);
		glUniform1f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].intensitat").c_str()), llums[i].intensitat);
	}

	glUniform3fv(glGetUniformLocation(shader.ID, "camPos"), 1, glm::value_ptr(camera.Position));

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

void DrawModels(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, std::vector<Llum>& llums, Camera& camera) {
	for (size_t i = 0; i < models.size(); ++i) {
		DrawModelWithShader(shader, models[i], modelMatrices[i], llums, camera);
	}
}

void DrawLights(Shader& shader, std::vector<Llum>& llums, Camera& camera) {
	for (Llum& llum : llums)
	{
		shader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(*llum.model));
		glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), llum.lightCol.r, llum.lightCol.g, llum.lightCol.b, llum.lightCol.a);
		llum.mesh->Draw(shader, camera);
	}
}
