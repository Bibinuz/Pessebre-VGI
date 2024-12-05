#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"
#include "Model.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"



enum MenuOption
{
	Menu,
	Juga,
	Manager,
	StaticCamera,
	Controls,
	Credits,
	Exit
};

class imGuiImplementation
{
public:
	imGuiImplementation(GLFWwindow* window);
	~imGuiImplementation();

	void imGuiInitNewFrame();
	void cameraSelector(std::vector<Camera>& Cameres, Camera*& c);
	void imGuiRender();
	void imGuiMainMenu(int windowWidth, int windowHeight);
	void imGuiShowFPS();
	void imGuiStaticCamera(Camera*& c, Camera& estaticCam) {c = &estaticCam; };
	void imGuiCamPosition(const Camera* camera);
	void imGuiControls(int windowWidth, int windowHeight);
	void imGuiCredits(int windowWidth, int windowHeight);
	void loadBackgroundImage(const char* imagePath);
	void RenderCenteredButton(const char* label, ImVec2 buttonSize);

	MenuOption op = Menu;

private:
	ImGuiIO io;
	unsigned int backgroundTextureID;
};

imGuiImplementation::imGuiImplementation(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	loadBackgroundImage("fonsMenu.png");
}

imGuiImplementation::~imGuiImplementation()
{
}

inline void imGuiImplementation::imGuiInitNewFrame()
{
	// Iniciem el nou frame d'ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}

inline void imGuiImplementation::cameraSelector(std::vector<Camera>& Cameres, Camera*& c)
{
	ImGui::Begin("Camera Selector");
	if (ImGui::Button("Camera 1")) {
		c = &Cameres[0];
	}
	if (ImGui::Button("Camera 2")) {
		c = &Cameres[1];
	}
	if (ImGui::Button("Camera 3")) {
		c = &Cameres[2];
	}
	ImGui::End();
}

inline void imGuiImplementation::imGuiRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

inline void imGuiImplementation::imGuiMainMenu(int windowWidth, int windowHeight)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	// Flags para impedir redimensionamiento, mover, etc.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

	// Crear la ventana de ImGui
	ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

	// ------------------- Título
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)(intptr_t)backgroundTextureID, windowSize);

	ImVec2 textSize = ImGui::CalcTextSize("El Pesebre");

	float centerX = (windowSize.x - textSize.x) / 2.0f;
	float centerYTitle = (windowSize.y - textSize.y) * 1.0f / 10.0f;

	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYTitle);
	ImGui::TextWrapped("El Pesebre");

	// ------------ Crear Botones
	ImVec2 buttonSize(200, 60);

	centerX = (windowSize.x - buttonSize.x) / 2.0f;
	float centerYTop = (windowSize.y - buttonSize.y) * 1.5f / 10.0f;
	float centerYCenter = (windowSize.y - buttonSize.y) * 2.5f / 10.0f;
	float centerYBotom = (windowSize.y - buttonSize.y) * 3.5f / 10.0f;
	float centerYControls = (windowSize.y - buttonSize.y) * 4.5f / 10.0f;
	float centerYCredits = (windowSize.y - buttonSize.y) * 5.5f / 10.0f;
	float centerYExit = (windowSize.y - buttonSize.y) * 6.5f / 10.0f;

	// Cambiar el color de los botones a azul oscuro
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.1f, 0.3f, 1.0f));        // Color del botón
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.2f, 0.5f, 1.0f)); // Color al pasar el ratón por encima
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.3f, 0.6f, 1.0f));  // Color al hacer clic

	// Crear los botones con el color aplicado
	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYTop);
	if (ImGui::Button("Jugar", buttonSize)) {
		op = Juga;
	}

	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYCenter);
	if (ImGui::Button("Manager", buttonSize)) {
		op = Manager;
	}

	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYBotom);
	if (ImGui::Button("Camera Estatica", buttonSize)) {
		op = StaticCamera;
	}

	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYControls);
	if (ImGui::Button("Controls", buttonSize)) {
		op = Controls;
	}

	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYCredits);
	if (ImGui::Button("Credits", buttonSize)) {
		op = Credits;
	}

	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYExit);
	if (ImGui::Button("Exit", buttonSize)) {
		op = Exit;
	}

	// Restaurar el color original
	ImGui::PopStyleColor(3);

	ImGui::End();
	imGuiRender();
}


inline void imGuiImplementation::imGuiShowFPS()
{
	static float timeElapsed = 0.0f;  // Tiempo acumulado
	static int frameCount = 0;  // Contador de frames
	static float fps = 0.0f;  // FPS promedio

	// Acumulamos el tiempo de cada frame
	timeElapsed += ImGui::GetIO().DeltaTime;  // DeltaTime es el tiempo entre frames

	// Incrementamos el contador de frames
	frameCount++;

	// Cada 5 segundos, calculamos la media de los FPS
	if (timeElapsed >= 1.0f) {
		fps = frameCount / timeElapsed;  // FPS = frames / tiempo
		timeElapsed = 0.0f;  // Reiniciamos el tiempo
		frameCount = 0;  // Reiniciamos el contador de frames
	}

	// Mostrar la ventana de FPS
	ImGui::SetNextWindowPos(ImVec2(10, 10));  // Posicionar la ventana en la esquina superior izquierda
	ImGui::Begin("FPS Counter", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Average FPS (last 1s): %.1f", fps);  // Mostrar el FPS promedio
	ImGui::End();
}

inline void imGuiImplementation::imGuiCamPosition(const Camera* camera)
{
	// Inicia una nova finestra d'ImGui
	ImGui::Begin("Camera Info");

	// Mostra les coordenades de la càmera
	ImGui::Text("Position:");
	ImGui::Text("X: %.2f", camera->Position.x);
	ImGui::Text("Y: %.2f", camera->Position.y);
	ImGui::Text("Z: %.2f", camera->Position.z);

	// Mostra la direcció (Orientation) de la càmera
	ImGui::Text("Orientation:");
	ImGui::Text("X: %.2f", camera->Orientation.x);
	ImGui::Text("Y: %.2f", camera->Orientation.y);
	ImGui::Text("Z: %.2f", camera->Orientation.z);

	// Mostra el vector Up
	ImGui::Text("Up:");
	ImGui::Text("X: %.2f", camera->Up.x);
	ImGui::Text("Y: %.2f", camera->Up.y);
	ImGui::Text("Z: %.2f", camera->Up.z);

	// Mostra si la càmera està activa o estàtica
	ImGui::Text("Camera Active: %s", camera->cameraActive ? "Yes" : "No");
	ImGui::Text("Static Camera: %s", camera->cameraEstatica ? "Yes" : "No");

	// Mostra la velocitat actual de la càmera
	ImGui::Text("Speed:");
	ImGui::Text("Current Speed: %.2f", camera->speed);
	ImGui::Text("Walk Speed: %.2f", camera->walkSpeed);
	ImGui::Text("Run Speed: %.2f", camera->runSpeed);

	// Finalitza la finestra
	ImGui::End();

}

inline void imGuiImplementation::imGuiControls(int windowWidth, int windowHeight)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	// Flags per impedir redimensionament, moure, etc.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

	// Crear la finestra de ImGui
	ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

	//-------------------Títol
	// Obtenir la mida de la finestra actual
	ImVec2 windowSize = ImGui::GetWindowSize();

	// Calcular la mida del text
	ImVec2 textSize = ImGui::CalcTextSize("Controls");

	// Calcular la posició X per centrar el text
	float centerX = (windowSize.x - textSize.x) / 2.0f;
	float centerYTitle = (windowSize.y - textSize.y) * 1.0f / 10.0f;

	// Posicionar el cursor en X per centrar-lo i Y a la part superior
	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYTitle); // Ajusta el valor de Y si és necessari
	ImGui::TextWrapped("Controls");

	//-------------------Controls del joc
	// Afegir espai entre el títol i els controls
	ImGui::Spacing();
	ImGui::Spacing();

	// Afegir el text dels controls del joc
	ImGui::Text("Controls del joc:");
	ImGui::BulletText("W, A, S, D: Moure's");
	ImGui::BulletText("Click esquerre: Interactuar amb botons");
	ImVec2 buttonSize(200, 60);
	if (ImGui::Button("Tornar al menu")) {
		op = Menu;
	}

	// Finalitzar la finestra
	ImGui::End();

	imGuiRender();
}

inline void imGuiImplementation::imGuiCredits(int windowWidth, int windowHeight)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	// Flags per impedir redimensionament, moure, etc.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

	// Crear la finestra de ImGui
	ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

	//-------------------Títol
	// Obtenir la mida de la finestra actual
	ImVec2 windowSize = ImGui::GetWindowSize();

	// Calcular la mida del text
	ImVec2 textSize = ImGui::CalcTextSize("Controls");

	// Calcular la posició X per centrar el text
	float centerX = (windowSize.x - textSize.x) / 2.0f;
	float centerYTitle = (windowSize.y - textSize.y) * 1.0f / 10.0f;

	// Posicionar el cursor en X per centrar-lo i Y a la part superior
	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYTitle); // Ajusta el valor de Y si és necessari
	ImGui::TextWrapped("Credits");

	//-------------------Controls del joc
	// Afegir espai entre el títol i els controls
	ImGui::Spacing();
	ImGui::Spacing();

	// Afegir el text dels controls del joc
	ImGui::Text("Desenvolupadors:");
	ImGui::BulletText("Gerard Purti");
	ImGui::BulletText("Jordi Viera");
	ImGui::BulletText("Biel Alavedra");
	ImGui::BulletText("Lucas Aviño");
	ImGui::BulletText("Marcal Armengol");
	ImGui::BulletText("Pere Llaurado");

	ImVec2 buttonSize(200, 60);
	if (ImGui::Button("Tornar al menu")) {
		op = Menu;
	}

	// Finalitzar la finestra
	ImGui::End();

	imGuiRender();
}

inline void imGuiImplementation::RenderCenteredButton(const char* label, ImVec2 buttonSize)
{
	// Obtener el tamaño de la ventana de ImGui
	ImVec2 windowSize = ImGui::GetWindowSize();

	// Calcular la posición X para centrar el botón horizontalmente
	float centerX = (windowSize.x - buttonSize.x) / 2.0f;

	// Establecer la posición del cursor en X para centrar el botón
	ImGui::SetCursorPosX(centerX);

	// Dibujar el botón centrado
	ImGui::Button(label, buttonSize);
}


void imGuiImplementation::loadBackgroundImage(const char* imagePath)
{
	glGenTextures(1, &backgroundTextureID);
	glBindTexture(GL_TEXTURE_2D, backgroundTextureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cerr << "Error: No s'ha pogut carregar la imatge: " << imagePath << std::endl;
		return;
	}

	std::cout << "Imatge carregada correctament: " << imagePath << std::endl;
	std::cout << "Dimensions: " << width << "x" << height << ", Canals: " << nrChannels << std::endl;

	if (width <= 0 || height <= 0) {
		std::cerr << "Error: Dimensions incorrectes per a la textura (width=" << width << ", height=" << height << ")." << std::endl;
		stbi_image_free(data); // Allibera la memòria
		return;
	}

	int maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	if (width > maxTextureSize || height > maxTextureSize) {
		std::cerr << "Error: La mida de la textura excedeix el límit de la GPU (màxim=" << maxTextureSize << ")." << std::endl;
		stbi_image_free(data); // Allibera la memòria
		return;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else {
		std::cerr << "Error: Nombre de canals no suportat (" << nrChannels << ")." << std::endl;
		stbi_image_free(data); // Allibera la memòria
		return;
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error després de glTexImage2D: " << error << std::endl;
		stbi_image_free(data); // Allibera la memòria
		return;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "Error després de glGenerateMipmap: " << error << std::endl;
	}

	stbi_image_free(data);
}




