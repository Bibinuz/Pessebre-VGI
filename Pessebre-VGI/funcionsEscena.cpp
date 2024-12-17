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
		glUniform1i(glGetUniformLocation(shader.ID, "diffuse0"), 0);
	}
	else {
		std::cerr << "No se han cargado texturas para el modelo." << std::endl;
	}
}

void DrawModelWithShader(Shader& shader, const Model& model, const glm::mat4& modelMatrix, Camera& camera) {

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
		{"models3d/terratopBaixRes.obj",	glm::vec3(0.0f, 0.0f, 0.0f),		glm::vec3(5.0f),	glm::radians(0.0f),		glm::vec3(0.0f, 1.0f, 0.0f)},
		{ "models3d/baltasarBaixRes.obj",	glm::vec3(-5.0f, 0.05f, -6.0f),		glm::vec3(20.0f),	glm::radians(-90.0f),	glm::vec3(1.0f, 0.0f, 0.0f) },
		{ "models3d/melchorBaixRes.obj",	glm::vec3(5.0f, 1.25f, -5.0f),		glm::vec3(2.0f),	glm::radians(-90.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/gasparBaixRes.obj",		glm::vec3(9.0f, 0.05f, -8.0f),		glm::vec3(20.0f),	glm::radians(-60.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/casaBaixRes.obj",		glm::vec3(1.0f, 3.75f, 1.75f),		glm::vec3(0.075f),	glm::radians(0.0f),		glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/joseBaixRes.obj",		glm::vec3(-2.0f, 1.75f, -2.0f),		glm::vec3(0.05f),	glm::radians(0.0f),		glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/mariaBaixRes.obj",		glm::vec3(1.0f, 0.05f, 0.0f),		glm::vec3(20.0f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/burroBaixRes.obj",		glm::vec3(0.5f, 0.05f, 2.0f),		glm::vec3(20.0f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/bueyBaixRes.obj",		glm::vec3(-0.5f, 0.05f, 2.0f),		glm::vec3(20.0f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/nenJesusBaixRes.obj",	glm::vec3(0.25f, 0.1f, 0.0f),		glm::vec3(20.0f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/cunaBaixRes.obj",		glm::vec3(0.25f, 0.05f, 1.0f),		glm::vec3(20.0f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/ovellaBaixRes.obj",		glm::vec3(-11.0f, 0.8f, -3.0f),		glm::vec3(0.4f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/camellBaixRes.obj",		glm::vec3(-6.0f, 0.0f, -13.0f),		glm::vec3(0.075f),	glm::radians(-40.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/camellBaixRes.obj",		glm::vec3(-11.0f, 1.5f, -17.0f),	glm::vec3(0.075f),	glm::radians(-70.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/camellBaixRes.obj",		glm::vec3(-9.0f, 0.0f, -15.0f),		glm::vec3(0.075f),	glm::radians(-60.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/fonsBaixRes.obj",		glm::vec3(0.0f, 27.7f, 0.0f),		glm::vec3(500.0f),	glm::radians(-180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/caganerBaixRes.obj",	glm::vec3(13.0f, 0.0f, 0.0f),		glm::vec3(5.0f),	glm::radians(-120.0f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ "models3d/angelBaixRes.obj",		glm::vec3(7.0f, 10.0f, 2.0f),		glm::vec3(1.5f),	glm::radians(180.0f),	glm::vec3(0.0f, 1.0f, 0.0f) }
	};

	// Crear cada modelo
	for (const auto& data : modelDataList) {
		models.emplace_back();  // Agregar un nuevo modelo
		modelMatrices.emplace_back();  // Agregar una nueva matriz de modelo
		CreateModel(models.back(), data.filePath, data.position, data.scale, modelMatrices.back(), data.rotationAngle, data.rotationAxis);
	}
}

void DrawModels(Shader& shader, const std::vector<Model>& models, const std::vector<glm::mat4>& modelMatrices, std::vector<Llum>& llums, Camera& camera) {

	shader.Activate(); 
	
	// He mogut tot aquest codi de DrawModelsWithShader a aqui perque les llums son les mateixes per a tots els objectes, no fa falta passarles totes cada vegada que es passen els uniforms al shader, augment considerable de fps
	glUniform1i(glGetUniformLocation(shader.ID, "numLights"), llums.size());

	for (int i = 0; i < llums.size(); i++)
	{
		glUniform1i(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].sw_light").c_str()),   llums[i].sw_light);
		glUniform3f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].lightPos").c_str()),   llums[i].lightPos.x, llums[i].lightPos.y, llums[i].lightPos.z);
		glUniform4f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].lightColor").c_str()), llums[i].lightCol.r, llums[i].lightCol.g, llums[i].lightCol.b, llums[i].lightCol.a);
		glUniform1i(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].tipus").c_str()),      llums[i].tipus);
		glUniform1f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].intensitat").c_str()), llums[i].intensitat);

		glActiveTexture(GL_TEXTURE0 + llums[i].shadowmap.unit);
		glBindTexture(GL_TEXTURE_2D, llums[i].shadowmap.depthMap);

		glUniform1i(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].depthMap").c_str()),   llums[i].shadowmap.unit);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].lightSpaceMatrix").c_str()), 1, GL_FALSE, glm::value_ptr(llums[i].shadowmap.lightSpaceMatrix));

	}

	//La posicio de camara tambe es la mateixa per a tots els objectes
	glUniform3fv(glGetUniformLocation(shader.ID, "camPos"), 1, glm::value_ptr(camera.Position)); 
	for (size_t i = 0; i < models.size(); ++i) {
		DrawModelWithShader(shader, models[i], modelMatrices[i], camera);
	}
}

void DrawLights(Shader& shader, std::vector<Llum>& llums, Camera& camera) {
	for (Llum& llum : llums)
	{
		if (llum.sw_light)
		{
			shader.Activate();
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(llum.model));
			glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), llum.lightCol.r, llum.lightCol.g, llum.lightCol.b, llum.lightCol.a);
			llum.mesh->Draw(shader, camera);
		}
	}
}