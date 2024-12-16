// imGuiImplementation.h

#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"
#include "Model.h"
#include <iostream>
#include "stb_image.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
    void imGuiMainMenu(int windowWidth, int windowHeight, Camera*& c);
    void imGuiShowFPS();
    void imGuiStaticCamera(Camera*& c, Camera& estaticCam) { c = &estaticCam; };
    void imGuiCamPosition(const Camera* camera);
    void imGuiControls(int windowWidth, int windowHeight);
    void imGuiCredits(int windowWidth, int windowHeight);
    void loadBackgroundImage(const char* imagePath);
    void loadMarcoImage(const char* imagePath);
    void RenderCenteredButton(const char* label, ImVec2 buttonSize);
    void rotateCameraAroundScene(Camera& camera, float radius, float speed);
    GLuint LoadTextureFromFile(const char* path);

    MenuOption op = Menu;

    // Ara el deixem públic per poder accedir-hi des de main.cpp
    unsigned int buttonReturnTextureID; // Nueva textura para "Tornar al menu"

private:
    ImGuiIO io;
    unsigned int backgroundTextureID;
    int backgroundWidth;
    int backgroundHeight;

    unsigned int marcoTextureID;
    int marcoWidth;
    int marcoHeight;

    unsigned int buttonJugarTextureID;
    unsigned int buttonManagerTextureID;
    unsigned int buttonStaticCameraTextureID;
    unsigned int buttonControlsTextureID;
    unsigned int buttonCreditsTextureID;
    unsigned int buttonExitTextureID;
};



imGuiImplementation::imGuiImplementation(GLFWwindow* window)
    : backgroundTextureID(0), backgroundWidth(0), backgroundHeight(0),
    marcoTextureID(0), marcoWidth(0), marcoHeight(0)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Cargar la imagen de fondo (título)
    loadBackgroundImage("Menu/pessebre2.png");
    // Cargar la imagen del marco
    loadMarcoImage("Menu/marco.png");

    // Carregar la textura del botó "Tornar al menu"
    buttonReturnTextureID = LoadTextureFromFile("Menu/back.png");
}

imGuiImplementation::~imGuiImplementation()
{
    // Eliminar la textura de fondo si se cargó correctamente
    if (backgroundTextureID != 0) {
        glDeleteTextures(1, &backgroundTextureID);
    }

    // Eliminar la textura del marco si se cargó correctamente
    if (marcoTextureID != 0) {
        glDeleteTextures(1, &marcoTextureID);
    }

    // Limpiar ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

inline void imGuiImplementation::imGuiInitNewFrame()
{
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

inline void imGuiImplementation::imGuiMainMenu(int windowWidth, int windowHeight, Camera*& c)
{
    // Carregar textures només una vegada
    static GLuint jugarTextureID = LoadTextureFromFile("Menu/jugar.png");
    static GLuint managerTextureID = LoadTextureFromFile("Menu/manager.png");
    static GLuint staticCameraTextureID = LoadTextureFromFile("Menu/estatica.png");
    static GLuint controlsTextureID = LoadTextureFromFile("Menu/controls.png");
    static GLuint creditsTextureID = LoadTextureFromFile("Menu/credits.png");
    static GLuint exitTextureID = LoadTextureFromFile("Menu/exit.png");

    rotateCameraAroundScene(*c, 20.0f, 0.2f);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

    ImVec2 windowSize = ImGui::GetWindowSize();

    // Dibujamos el marco a tamaño completo de la ventana
    if (marcoTextureID != 0) {
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image((ImTextureID)(uintptr_t)marcoTextureID, windowSize);
    }

    // ------------------- Títol com a imatge
    if (backgroundTextureID != 0) {
        float imageWidth = windowSize.x * 0.3f;
        float aspectRatio = (backgroundHeight > 0) ? static_cast<float>(backgroundHeight) / static_cast<float>(backgroundWidth) : 1.0f;
        float imageHeight = imageWidth * aspectRatio;
        ImVec2 imageSize(imageWidth, imageHeight);

        float centerX = (windowSize.x - imageSize.x) / 2.0f;
        float centerYTitle = (windowSize.y - imageSize.y) * 1.0f / 10.0f;

        ImGui::SetCursorPosX(centerX);
        ImGui::SetCursorPosY(centerYTitle);
        ImGui::Image((ImTextureID)(uintptr_t)backgroundTextureID, imageSize);
    }

    // ------------ Crear Botons amb Imatges en 2 columnes x 3 files
    ImVec2 buttonSize(300, 100); // Amplada = 300, Alçada = 90
    float spacing = 30.0f;      // Espai vertical entre botons
    float horizontalSpacing = 70.0f; // Espai horizontal entre columnes

    // Calculem la posició inicial
    float totalHeight = (buttonSize.y * 3) + (spacing * 2); // 3 botons + 2 espais
    float startY = (windowSize.y - totalHeight) / 2.0f;

    float totalWidth = (buttonSize.x * 2) + horizontalSpacing;
    float startX = (windowSize.x - totalWidth) / 2.0f;

    // Ajustar estil per eliminar fons i marges dels botons
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    // Primera columna
    ImGui::SetCursorPos(ImVec2(startX, startY));
    if (ImGui::ImageButton("jugar_btn", (ImTextureID)(uintptr_t)jugarTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) { op = Juga; }

    ImGui::SetCursorPos(ImVec2(startX, startY + buttonSize.y + spacing));
    if (ImGui::ImageButton("manager_btn", (ImTextureID)(uintptr_t)managerTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) { op = Manager; }

    ImGui::SetCursorPos(ImVec2(startX, startY + (buttonSize.y + spacing) * 2));
    if (ImGui::ImageButton("camera_btn", (ImTextureID)(uintptr_t)staticCameraTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) { op = StaticCamera; }

    // Segona columna
    float secondColumnX = startX + buttonSize.x + horizontalSpacing;

    ImGui::SetCursorPos(ImVec2(secondColumnX, startY));
    if (ImGui::ImageButton("controls_btn", (ImTextureID)(uintptr_t)controlsTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) { op = Controls; }

    ImGui::SetCursorPos(ImVec2(secondColumnX, startY + buttonSize.y + spacing));
    if (ImGui::ImageButton("credits_btn", (ImTextureID)(uintptr_t)creditsTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) { op = Credits; }

    ImGui::SetCursorPos(ImVec2(secondColumnX, startY + (buttonSize.y + spacing) * 2));
    if (ImGui::ImageButton("exit_btn", (ImTextureID)(uintptr_t)exitTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) { op = Exit; }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImGui::End();
    imGuiRender();
}


inline void imGuiImplementation::imGuiShowFPS()
{
    static float timeElapsed = 0.0f;
    static int frameCount = 0;
    static float fps = 0.0f;

    timeElapsed += ImGui::GetIO().DeltaTime;
    frameCount++;

    if (timeElapsed >= 1.0f) {
        fps = frameCount / timeElapsed;
        timeElapsed = 0.0f;
        frameCount = 0;
    }

    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::Begin("FPS Counter", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Average FPS (last 1s): %.1f", fps);
    ImGui::End();
}

inline void imGuiImplementation::imGuiCamPosition(const Camera* camera)
{
    ImGui::Begin("Camera Info");
    ImGui::Text("Position:");
    ImGui::Text("X: %.2f", camera->Position.x);
    ImGui::Text("Y: %.2f", camera->Position.y);
    ImGui::Text("Z: %.2f", camera->Position.z);

    ImGui::Text("Orientation:");
    ImGui::Text("X: %.2f", camera->Orientation.x);
    ImGui::Text("Y: %.2f", camera->Orientation.y);
    ImGui::Text("Z: %.2f", camera->Orientation.z);

    ImGui::Text("Up:");
    ImGui::Text("X: %.2f", camera->Up.x);
    ImGui::Text("Y: %.2f", camera->Up.y);
    ImGui::Text("Z: %.2f", camera->Up.z);

    ImGui::Text("Camera Active: %s", camera->cameraActive ? "Yes" : "No");
    ImGui::Text("Static Camera: %s", camera->cameraEstatica ? "Yes" : "No");

    ImGui::Text("Speed:");
    ImGui::Text("Current Speed: %.2f", camera->speed);
    ImGui::Text("Walk Speed: %.2f", camera->walkSpeed);
    ImGui::Text("Run Speed: %.2f", camera->runSpeed);

    ImGui::End();
}

inline void imGuiImplementation::imGuiControls(int windowWidth, int windowHeight)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize = ImGui::CalcTextSize("Controls");

    float centerX = (windowSize.x - textSize.x) / 2.0f;
    float centerYTitle = (windowSize.y - textSize.y) * 1.0f / 10.0f;

    ImGui::SetCursorPosX(centerX);
    ImGui::SetCursorPosY(centerYTitle);
    ImGui::TextWrapped("Controls");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Controls del juego:");
    ImGui::BulletText("W, A, S, D: Moverse");
    ImGui::BulletText("Click izquierdo: Interactuar con botones");

    // Botó de tornar al menú com a imatge
    ImVec2 buttonSize(100, 100);
    float buttonCenterX = (windowSize.x - buttonSize.x) / 2.0f;
    float buttonCenterY = (windowSize.y - buttonSize.y) * 8.0f / 10.0f;
    ImGui::SetCursorPos(ImVec2(buttonCenterX, buttonCenterY));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    if (ImGui::ImageButton("return_btn_controls", (ImTextureID)(uintptr_t)buttonReturnTextureID, buttonSize, ImVec2(0, 0), ImVec2(1, 1))) {
        op = Menu;
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImGui::End();
    imGuiRender();
}

inline void imGuiImplementation::imGuiCredits(int windowWidth, int windowHeight)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 creditsTextSize = ImGui::CalcTextSize("Credits");
    float centerX = (windowSize.x - creditsTextSize.x) / 2.0f;
    float centerY = (windowSize.y - creditsTextSize.y) / 3.0f;

    ImGui::SetWindowFontScale(3);
    ImGui::SetCursorPos(ImVec2(centerX, centerY));
    ImGui::Text("Credits");

    ImVec2 devsTextSize = ImGui::CalcTextSize("Desenvolupadors:");
    float devsCenterX = (windowSize.x - devsTextSize.x) / 2.0f;
    float devsCenterY = centerY + creditsTextSize.y + 20;

    ImGui::SetCursorPos(ImVec2(devsCenterX - 60, devsCenterY));
    ImGui::Text("Desenvolupadors:");

    float listCenterX = windowSize.x / 2.0f - 100.0f;
    float listStartY = devsCenterY + devsTextSize.y + 10;

    ImGui::SetCursorPos(ImVec2(listCenterX, listStartY));
    ImGui::BulletText("Gerard Purti");
    ImGui::SetCursorPos(ImVec2(listCenterX, listStartY + 35));
    ImGui::BulletText("Jordi Viera");
    ImGui::SetCursorPos(ImVec2(listCenterX, listStartY + 70));
    ImGui::BulletText("Biel Alavedra");
    ImGui::SetCursorPos(ImVec2(listCenterX, listStartY + 105));
    ImGui::BulletText("Lucas Avino");
    ImGui::SetCursorPos(ImVec2(listCenterX, listStartY + 140));
    ImGui::BulletText("Marcal Armengol");
    ImGui::SetCursorPos(ImVec2(listCenterX, listStartY + 175));
    ImGui::BulletText("Pere Llaurado");

    ImGui::SetWindowFontScale(2);
    ImVec2 buttonSizeCredits(100, 100);
    float buttonCenterX = (windowSize.x - buttonSizeCredits.x) / 2.0f;
    float buttonCenterY = listStartY + 240;
    ImGui::SetCursorPos(ImVec2(buttonCenterX, buttonCenterY));

    // Botó de tornar al menú com a imatge
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    if (ImGui::ImageButton("return_btn_credits", (ImTextureID)(uintptr_t)buttonReturnTextureID, buttonSizeCredits, ImVec2(0, 0), ImVec2(1, 1))) {
        op = Menu;
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    ImGui::End();
    imGuiRender();
}

inline void imGuiImplementation::RenderCenteredButton(const char* label, ImVec2 buttonSize)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    float centerX = (windowSize.x - buttonSize.x) / 2.0f;
    ImGui::SetCursorPosX(centerX);
    ImGui::Button(label, buttonSize);
}

void imGuiImplementation::loadBackgroundImage(const char* imagePath)
{
    glGenTextures(1, &backgroundTextureID);
    glBindTexture(GL_TEXTURE_2D, backgroundTextureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Error: No se pudo cargar la imagen: " << imagePath << std::endl;
        return;
    }

    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Dimensiones incorrectas para la textura." << std::endl;
        stbi_image_free(data);
        return;
    }

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    if (width > maxTextureSize || height > maxTextureSize) {
        std::cerr << "Error: El tamaño de la textura excede el límite de la GPU." << std::endl;
        stbi_image_free(data);
        return;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    backgroundWidth = width;
    backgroundHeight = height;

    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cerr << "Error: Número de canales no soportado (" << nrChannels << ")." << std::endl;
        stbi_image_free(data);
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void imGuiImplementation::loadMarcoImage(const char* imagePath)
{
    glGenTextures(1, &marcoTextureID);
    glBindTexture(GL_TEXTURE_2D, marcoTextureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Error: No se pudo cargar la imagen del marco: " << imagePath << std::endl;
        return;
    }

    marcoWidth = width;
    marcoHeight = height;

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
        std::cerr << "Error: Número de canales no soportado para el marco (" << nrChannels << ")." << std::endl;
        stbi_image_free(data);
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void imGuiImplementation::rotateCameraAroundScene(Camera& camera, float radius, float speed)
{
    static float angle = 0.0f;

    float x = radius * cos(angle);
    float z = radius * sin(angle);

    camera.Position = glm::vec3(x, camera.Position.y, z);
    camera.Orientation = glm::normalize(glm::vec3(-x, 0.0f, -z));

    angle += speed * ImGui::GetIO().DeltaTime;
    if (angle > 2 * M_PI) {
        angle -= 2 * M_PI;
    }
}


GLuint imGuiImplementation::LoadTextureFromFile(const char* path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Paràmetres de la textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
