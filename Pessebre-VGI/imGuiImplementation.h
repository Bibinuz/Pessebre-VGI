#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "Camera.h"
#include "Model.h"
#include <iostream> // Añadido para manejo de entradas y salidas

// Asegúrate de incluir el encabezado de tu cargador de OpenGL aquí si es necesario.
// Por ejemplo, si usas GLAD, podrías tener:
// #include <glad/glad.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// No definimos STB_IMAGE_IMPLEMENTATION aquí para evitar múltiples definiciones
#include "stb_image.h" // Incluido sin definir STB_IMAGE_IMPLEMENTATION

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
    void RenderCenteredButton(const char* label, ImVec2 buttonSize);
    void rotateCameraAroundScene(Camera& camera, float radius, float speed);
    void renderScene(GLFWwindow* window);

    MenuOption op = Menu;
    //Camera cam;

private:
    ImGuiIO io;
    unsigned int backgroundTextureID;
    int backgroundWidth;  // Ancho de la imagen de fondo
    int backgroundHeight; // Alto de la imagen de fondo

    // Nuevos identificadores de textura para los botones
    unsigned int buttonJugarTextureID;
    unsigned int buttonManagerTextureID;
    unsigned int buttonStaticCameraTextureID;
    unsigned int buttonControlsTextureID;
    unsigned int buttonCreditsTextureID;
    unsigned int buttonExitTextureID;
};

imGuiImplementation::imGuiImplementation(GLFWwindow* window)
    : backgroundTextureID(0), backgroundWidth(0), backgroundHeight(0) // Inicialización de variables
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Cargar la imagen de fondo (título) desde la ruta especificada
    loadBackgroundImage("C:/Users/gfxgp/Desktop/Pessebre-VGI (3)/Pessebre-VGI/Pessebre-VGI/Menu/pessebre.png");
}

imGuiImplementation::~imGuiImplementation()
{
    // Eliminar la textura si se cargó correctamente
    if (backgroundTextureID != 0) {
        glDeleteTextures(1, &backgroundTextureID);
    }

    // Limpiar ImGui
    //dfjgwhh
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

inline void imGuiImplementation::imGuiInitNewFrame()
{
    // Iniciar el nuevo frame de ImGui
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

    // Flags para impedir redimensionamiento, mover, etc.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

    // Crear la ventana de ImGui
    ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

    // ------------------- Título como imagen
    if (backgroundTextureID != 0) {
        ImVec2 windowSize = ImGui::GetWindowSize();

        // Definir el tamaño de la imagen como el 30% del ancho de la ventana
        float imageWidth = windowSize.x * 0.3f; // Ajusta el factor de escala según tus necesidades
        float aspectRatio = (backgroundHeight > 0) ? static_cast<float>(backgroundHeight) / static_cast<float>(backgroundWidth) : 1.0f;
        float imageHeight = imageWidth * aspectRatio;

        // Definir el tamaño de la imagen con el escalado
        ImVec2 imageSize(imageWidth, imageHeight);

        // Calcular la posición para centrar la imagen
        float centerX = (windowSize.x - imageSize.x) / 2.0f;
        float centerYTitle = (windowSize.y - imageSize.y) * 1.0f / 10.0f;

        // Establecer la posición del cursor para la imagen
        ImGui::SetCursorPosX(centerX);
        ImGui::SetCursorPosY(centerYTitle);

        // Dibujar la imagen escalada
        // Asegúrate de que el tipo de ImTextureID sea compatible. Si sigue habiendo errores, considera redefinir ImTextureID.
        ImGui::Image(static_cast<ImTextureID>(backgroundTextureID), imageSize);
    }

    // ------------ Crear Botones
    ImVec2 buttonSize(200, 60);

    ImVec2 windowSize = ImGui::GetWindowSize();
    float centerX = (windowSize.x - buttonSize.x) / 2.0f;
    float centerYTop = (windowSize.y - buttonSize.y) * 3.0f / 10.0f;
    float centerYCenter = (windowSize.y - buttonSize.y) * 4.0f / 10.0f;
    float centerYBotom = (windowSize.y - buttonSize.y) * 5.0f / 10.0f;
    float centerYControls = (windowSize.y - buttonSize.y) * 6.0f / 10.0f;
    float centerYCredits = (windowSize.y - buttonSize.y) * 7.0f / 10.0f;
    float centerYExit = (windowSize.y - buttonSize.y) * 8.0f / 10.0f;

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

    // Cada 1 segundo, calculamos la media de los FPS
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
    // Iniciar una nueva ventana de ImGui
    ImGui::Begin("Camera Info");

    // Mostrar las coordenadas de la cámara
    ImGui::Text("Position:");
    ImGui::Text("X: %.2f", camera->Position.x);
    ImGui::Text("Y: %.2f", camera->Position.y);
    ImGui::Text("Z: %.2f", camera->Position.z);

    // Mostrar la dirección (Orientation) de la cámara
    ImGui::Text("Orientation:");
    ImGui::Text("X: %.2f", camera->Orientation.x);
    ImGui::Text("Y: %.2f", camera->Orientation.y);
    ImGui::Text("Z: %.2f", camera->Orientation.z);

    // Mostrar el vector Up
    ImGui::Text("Up:");
    ImGui::Text("X: %.2f", camera->Up.x);
    ImGui::Text("Y: %.2f", camera->Up.y);
    ImGui::Text("Z: %.2f", camera->Up.z);

    // Mostrar si la cámara está activa o estática
    ImGui::Text("Camera Active: %s", camera->cameraActive ? "Yes" : "No");
    ImGui::Text("Static Camera: %s", camera->cameraEstatica ? "Yes" : "No");

    // Mostrar la velocidad actual de la cámara
    ImGui::Text("Speed:");
    ImGui::Text("Current Speed: %.2f", camera->speed);
    ImGui::Text("Walk Speed: %.2f", camera->walkSpeed);
    ImGui::Text("Run Speed: %.2f", camera->runSpeed);

    // Finalizar la ventana
    ImGui::End();
}

inline void imGuiImplementation::imGuiControls(int windowWidth, int windowHeight)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Flags para impedir redimensionamiento, mover, etc.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    // Crear la ventana de ImGui
    ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

    //-------------------Títol
    // Obtener el tamaño de la ventana actual
    ImVec2 windowSize = ImGui::GetWindowSize();

    // Calcular el tamaño del texto
    ImVec2 textSize = ImGui::CalcTextSize("Controls");

    // Calcular la posición X para centrar el texto
    float centerX = (windowSize.x - textSize.x) / 2.0f;
    float centerYTitle = (windowSize.y - textSize.y) * 1.0f / 10.0f;

    // Posicionar el cursor en X para centrarlo y Y en la parte superior
    ImGui::SetCursorPosX(centerX);
    ImGui::SetCursorPosY(centerYTitle); // Ajusta el valor de Y si es necesario
    ImGui::TextWrapped("Controls");

    //-------------------Controls del juego
    // Añadir espacio entre el título y los controles
    ImGui::Spacing();
    ImGui::Spacing();

    // Añadir el texto de los controles del juego
    ImGui::Text("Controls del juego:");
    ImGui::BulletText("W, A, S, D: Moverse");
    ImGui::BulletText("Click izquierdo: Interactuar con botones");
    ImVec2 buttonSize(200, 60);
    if (ImGui::Button("Tornar al menu")) {
        op = Menu;
    }

    // Finalizar la ventana
    ImGui::End();

    imGuiRender();
}

inline void imGuiImplementation::imGuiCredits(int windowWidth, int windowHeight)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Flags para impedir redimensionamiento, mover, etc.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    // Crear la ventana de ImGui
    ImGui::Begin("Ventana a Pantalla Completa", nullptr, windowFlags);

    // Obtener el tamaño de la ventana
    ImVec2 windowSize = ImGui::GetWindowSize();

    // ------------------- Texto "Credits" centrado
    ImVec2 creditsTextSize = ImGui::CalcTextSize("Credits");
    float centerX = (windowSize.x - creditsTextSize.x) / 2.0f; // Centrado horizontalmente
    float centerY = (windowSize.y - creditsTextSize.y) / 3.0f; // Ajuste vertical para situarlo más arriba
    ImGui::SetWindowFontScale(3);
    ImGui::SetCursorPos(ImVec2(centerX, centerY));
    ImGui::Text("Credits");

    // ------------------- Texto "Desenvolupadors:" centrado debajo de "Credits"
    ImVec2 devsTextSize = ImGui::CalcTextSize("Desenvolupadors:");
    float devsCenterX = (windowSize.x - devsTextSize.x) / 2.0f;
    float devsCenterY = centerY + creditsTextSize.y + 20; // Añadir espacio debajo de "Credits"

    ImGui::SetCursorPos(ImVec2(devsCenterX - 60, devsCenterY));
    ImGui::Text("Desenvolupadors:");

    // ------------------- Lista de desarrolladores
    float listCenterX = windowSize.x / 2.0f - 100.0f; // Ajuste horizontal para que quede cerca del texto
    float listStartY = devsCenterY + devsTextSize.y + 10; // Espacio debajo de "Desenvolupadors:"

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

    // ------------------- Botón centrado
    ImGui::SetWindowFontScale(2);
    ImVec2 buttonSizeCredits(300, 60);
    float buttonCenterX = (windowSize.x - buttonSizeCredits.x) / 2.0f;
    float buttonCenterY = listStartY + 240; // Ajuste vertical debajo de la lista
    ImGui::SetCursorPos(ImVec2(buttonCenterX, buttonCenterY));
    if (ImGui::Button("Tornar al menu", buttonSizeCredits)) {
        op = Menu;
    }

    // Finalizar la ventana
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
        std::cerr << "Error: No se pudo cargar la imagen: " << imagePath << std::endl;
        return;
    }

    std::cout << "Imagen cargada correctamente: " << imagePath << std::endl;
    std::cout << "Dimensiones: " << width << "x" << height << ", Canales: " << nrChannels << std::endl;

    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Dimensiones incorrectas para la textura (width=" << width << ", height=" << height << ")." << std::endl;
        stbi_image_free(data); // Liberar la memoria
        return;
    }

    int maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    if (width > maxTextureSize || height > maxTextureSize) {
        std::cerr << "Error: El tamaño de la textura excede el límite de la GPU (máximo=" << maxTextureSize << ")." << std::endl;
        stbi_image_free(data); // Liberar la memoria
        return;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Guardar las dimensiones de la imagen
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
        stbi_image_free(data); // Liberar la memoria
        return;
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error después de glTexImage2D: " << error << std::endl;
        stbi_image_free(data); // Liberar la memoria
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Error después de glGenerateMipmap: " << error << std::endl;
    }

    stbi_image_free(data);
}

void imGuiImplementation::rotateCameraAroundScene(Camera& camera, float radius, float speed)
{
    static float angle = 0.0f; // Ángulo inicial

    // Calcula las nuevas coordenadas de la cámara
    float x = radius * cos(angle);
    float z = radius * sin(angle);

    // Establece la nueva posición de la cámara
    camera.Position = glm::vec3(x, camera.Position.y, z);

    // Actualiza la orientación de la cámara para mirar hacia el centro de la escena (0,0,0)
    camera.Orientation = glm::normalize(glm::vec3(-x, 0.0f, -z));

    // Incrementa el ángulo según la velocidad especificada
    angle += speed * ImGui::GetIO().DeltaTime;

    // Asegura que el ángulo no exceda los 360 grados
    if (angle > 2 * M_PI) {
        angle -= 2 * M_PI;
    }
}

void imGuiImplementation::renderScene(GLFWwindow* window)
{
    // Implementación de renderizado de la escena si es necesario
}
