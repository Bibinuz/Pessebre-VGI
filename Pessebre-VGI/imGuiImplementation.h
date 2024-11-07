#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"

class imGuiImplementation
{
public:
	imGuiImplementation(GLFWwindow* window);
	~imGuiImplementation();

	void imGuiInitNewFrame();
	Camera* cameraSelector(std::vector<Camera> Cameres);

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

inline Camera* imGuiImplementation::cameraSelector(std::vector<Camera> Cameres)
{
	Camera* camera  = nullptr;
	ImGui::Begin("Camera Selector");
	if (!Cameres.empty())
	{
		if (ImGui::Button("Camera 1")) {
			camera = &Cameres[0];
		}
		if (ImGui::Button("Camera 2")) {
			camera = &Cameres[1];
		}
		if (ImGui::Button("Camera 3")) {
			camera = &Cameres[2];
		}
	}
	else {
		ImGui::Text("No hi ha càmeres disponibles.");
	}
	ImGui::End();

	return camera;
}
