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
    // Основные векторы камеры
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    // Предотвращает "прыжки" камеры при первом клике мыши
    bool firstClick = true;

    // Ширина и высота окна
    int width;
    int height;

    // Настройка скорости и чувствительности камеры
    float speed = 0.001f; // Уменьшенная скорость
    float sensitivity = 100.0f;

    // Конструктор камеры для установки начальных значений
    Camera(int width, int height, glm::vec3 position);

    // Обновляет матрицу камеры для вершинного шейдера
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    // Экспортирует матрицу камеры в шейдер
    void Matrix(Shader& shader, const char* uniform);
    // Обрабатывает ввод для управления камерой
    void Inputs(GLFWwindow* window);
};

#endif
