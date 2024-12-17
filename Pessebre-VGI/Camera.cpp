#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 minPos, glm::vec3 maxPos)
{
    this->width = width;
    this->height = height;
    this->Position = position;
    this->minPosition = minPos;
    this->maxPosition = maxPos;
    this->speed = walkSpeed;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    // Inicializa las matrices para que no sean matrices nulas
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Hace que la c�mara mire en la direcci�n correcta desde la posici�n correcta
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Agrega perspectiva a la escena
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Establece la nueva matriz de la c�mara
    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    // Exporta la matriz de la c�mara
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
    glm::vec3 newPosition = Position;

    // Movimiento en las direcciones X, Y, Z
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newPosition += speed * Orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newPosition += speed * -glm::normalize(glm::cross(Orientation, Up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newPosition += speed * -Orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newPosition += speed * glm::normalize(glm::cross(Orientation, Up));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        newPosition += speed * Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        newPosition += speed * -Up;

    // Restringe el movimiento dentro de los l�mites
    newPosition.x = glm::clamp(newPosition.x, minPosition.x, maxPosition.x);
    newPosition.y = glm::clamp(newPosition.y, minPosition.y, maxPosition.y);
    newPosition.z = glm::clamp(newPosition.z, minPosition.z, maxPosition.z);

    Position = newPosition;

    // Control de velocidad
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        speed = runSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
        speed = walkSpeed;

    // Restante de la l�gica de inputs
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cameraActive = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        cameraActive = true;
    }
    if (cameraActive)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
        {
            Orientation = newOrientation;
        }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        glfwSetCursorPos(window, width / 2, height / 2);
    }
}

void Camera::RotateCamera(float angle, const glm::vec3& axis)
{
    // Convertir el �ngulo a radianes
    float radians = glm::radians(angle);

    // Crear la matriz de rotaci�n
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, axis);

    // Aplicar la rotaci�n a la orientaci�n de la c�mara
    Orientation = glm::vec3(rotationMatrix * glm::vec4(Orientation, 0.0f));
}
