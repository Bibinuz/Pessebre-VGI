#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"
#include "Model.h"
#include <iostream> // Añadido para manejo de entradas y salidas
#include "stb_image.h" // Incluido sin definir STB_IMAGE_IMPLEMENTATION

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
    void loadMarcoImage(const char* imagePath); // Nueva función para cargar el marco
    void RenderCenteredButton(const char* label, ImVec2 buttonSize);
    void rotateCameraAroundScene(Camera& camera, float radius, float speed);
    void renderScene(GLFWwindow* window);

    MenuOption op = Menu;

private:
    ImGuiIO io;
    unsigned int backgroundTextureID;
    int backgroundWidth;  // Ancho de la imagen de fondo
    int backgroundHeight; // Alto de la imagen de fondo

    unsigned int marcoTextureID;  // Textura del marco
    int marcoWidth;               // Ancho del marco
    int marcoHeight;              // Alto del marco

    // Identificadores de textura para los botones (no usados actualmente, pero definidos)
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

    // ------------------- Título como imagen
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

    // ------------ Crear Botones en 2 columnas x 3 filas
    ImVec2 buttonSize(200, 60);

    // Cambiar el color de los botones a azul oscuro
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.1f, 0.3f, 1.0f));        // Color del botón
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.2f, 0.5f, 1.0f)); // Color al pasar el ratón por encima
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.3f, 0.6f, 1.0f));  // Color al hacer clic

    // Vamos a colocar 6 botones en 2 columnas y 3 filas:
    // Columna 1: Jugar, Manager, Camera Estatica
    // Columna 2: Controls, Credits, Exit

    float spacing = 20.0f; // Espacio vertical entre botones
    float totalHeight = (buttonSize.y * 3) + (spacing * 2); // Altura total de 3 botones + espacios
    float startY = (windowSize.y - totalHeight) / 2.0f;     // Posición Y inicial centrada

    float horizontalSpacing = 50.0f; // Espacio horizontal entre columnas
    float totalWidth = (buttonSize.x * 2) + horizontalSpacing;
    float startX = (windowSize.x - totalWidth) / 2.0f; // Posición X inicial centrada para primera columna

    // Primera columna
    ImGui::SetCursorPos(ImVec2(startX, startY));
    if (ImGui::Button("Jugar", buttonSize)) { op = Juga; }

    ImGui::SetCursorPos(ImVec2(startX, startY + buttonSize.y + spacing));
    if (ImGui::Button("Manager", buttonSize)) { op = Manager; }

    ImGui::SetCursorPos(ImVec2(startX, startY + (buttonSize.y + spacing) * 2));
    if (ImGui::Button("Camera Estatica", buttonSize)) { op = StaticCamera; }

    // Segunda columna (a la derecha de la primera)
    float secondColumnX = startX + buttonSize.x + horizontalSpacing;

    ImGui::SetCursorPos(ImVec2(secondColumnX, startY));
    if (ImGui::Button("Controls", buttonSize)) { op = Controls; }

    ImGui::SetCursorPos(ImVec2(secondColumnX, startY + buttonSize.y + spacing));
    if (ImGui::Button("Credits", buttonSize)) { op = Credits; }

    ImGui::SetCursorPos(ImVec2(secondColumnX, startY + (buttonSize.y + spacing) * 2));
    if (ImGui::Button("Exit", buttonSize)) { op = Exit; }

    // Restaurar el color original de los botones
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
    ImVec2 buttonSize(200, 60);
    if (ImGui::Button("Tornar al menu")) {
        op = Menu;
    }

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
    ImVec2 buttonSizeCredits(300, 60);
    float buttonCenterX = (windowSize.x - buttonSizeCredits.x) / 2.0f;
    float buttonCenterY = listStartY + 240;
    ImGui::SetCursorPos(ImVec2(buttonCenterX, buttonCenterY));
    if (ImGui::Button("Tornar al menu", buttonSizeCredits)) {
        op = Menu;
    }

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

void imGuiImplementation::renderScene(GLFWwindow* window)
{
    // Implementación de renderizado de la escena si es necesario
}