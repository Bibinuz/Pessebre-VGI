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
	void cameraSelector(std::vector<Camera>& Cameres, Camera*& c);
	void imGuiRender();

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
