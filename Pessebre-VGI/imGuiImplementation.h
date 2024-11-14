#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"

enum MenuOption
{
	None,
	Juga,
	Manager,
	StaticCamera
};

class imGuiImplementation
{
public:
	imGuiImplementation(GLFWwindow* window);
	~imGuiImplementation();

	void imGuiInitNewFrame();
	void cameraSelector(std::vector<Camera>& Cameres, Camera*& c);
	void imGuiRender();
	void imGuiMainMenu();
	void imGuiShowFPS();

	MenuOption op = None;

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

inline void imGuiImplementation::imGuiMainMenu()
{
	// Comença una nova finestra per al menú
	ImGui::Begin("Main Menu");

	// Botons de menú
	if (ImGui::Button("Juga")) {
		op = Juga;  // Canvia l'estat quan es fa clic en "Juga"
	}
	if (ImGui::Button("Manager")) {
		op = Manager;  // Canvia l'estat quan es fa clic en "Manager"
	}
	if (ImGui::Button("Ca    mera Estatica")) {
		op = StaticCamera;  // Canvia l'estat quan es fa clic en "Càmera Estàtica"
	}

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




