#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"
#include "Model.h"

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

	void RenderCenteredButton(const char* label, ImVec2 buttonSize);

	MenuOption op = Menu;

private:
	ImGuiIO io;
};

imGuiImplementation::imGuiImplementation(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
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

inline void imGuiImplementation::imGuiMainMenu(int windowWidth,int windowHeight)
{
	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

	
	// Flags para impedir redimensionamiento, mover, etc.
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;


	// Crear la ventana de ImGui
	ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);
	//-------------------Titul
	 // Obtener el tamaño de la ventana actual
	ImVec2 windowSize = ImGui::GetWindowSize();

	// Calcular el tamaño del texto
	ImVec2 textSize = ImGui::CalcTextSize("El Pesebre");

	// Calcular la posición X para centrar el texto
	float centerX = (windowSize.x - textSize.x) / 2.0f;
	float centerYTitle = (windowSize.y - textSize.y) * 1.0f / 10.0f;
	// Posicionar el cursor en X para centrarlo y Y en la parte superior
	ImGui::SetCursorPosX(centerX);
	ImGui::SetCursorPosY(centerYTitle); // Ajusta el valor de Y si es necesario
	ImGui::TextWrapped("El Pesebre");
	 
	//------------Crear Botons
	ImVec2 buttonSize(200, 60);
	// Obtener el tamaño de la ventana de ImGui

	centerX = (windowSize.x - buttonSize.x) / 2.0f;
	float centerYTop = (windowSize.y - buttonSize.y)*1.5f / 10.0f;
	float centerYCenter = (windowSize.y - buttonSize.y) * 2.5f / 10.0f;
	float centerYBotom = (windowSize.y - buttonSize.y) * 3.5f / 10.0f;
	float centerYControls = (windowSize.y - buttonSize.y) * 4.5f / 10.0f;
	float centerYCredits = (windowSize.y - buttonSize.y) * 5.5f / 10.0f;
	float centerYExit = (windowSize.y - buttonSize.y) * 6.5f / 10.0f;




	// Establecer la posición del cursor en X para centrar el botón
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
	// Contenido de la ventana
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

	// Finalitzar la finestra
	ImGui::End();

	imGuiRender();
}

inline void imGuiImplementation::imGuiCredits(int windowWidth, int windowHeight)
{

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

