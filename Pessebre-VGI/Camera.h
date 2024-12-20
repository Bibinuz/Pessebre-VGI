#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    bool cameraActive = true;

    int width;
    int height;

    float speed;
    float walkSpeed = 0.1f;
    float runSpeed = 4 * walkSpeed;
    float sensitivity = 100.0f;

    // L�mites de movimiento de la c�mara
    glm::vec3 minPosition;
    glm::vec3 maxPosition;

    Camera(int width, int height, glm::vec3 position, glm::vec3 minPos, glm::vec3 maxPos);

    void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);
    void Inputs(GLFWwindow* window);
    void RotateCamera(float angle, const glm::vec3& axis);
};

#endif // CAMERA_CLASS_H
