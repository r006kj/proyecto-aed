//
// Objective: Render a movable 3D Cube
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Cube.h"
#include "Octree.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Octree* globalOctree = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (globalOctree) {
            // Genera coordenadas dentro del rango del Octree (0 a 100)
            int rx = rand() % 101;
            int ry = rand() % 101;
            int rz = rand() % 101;

            globalOctree->insert(rx, ry, rz);
            std::cout << "Punto insertado: (" << rx << ", " << ry << ", " << rz << ")\n";
        }
    }
}

struct OrbitCamera {
    glm::vec3 target = glm::vec3(50.0f, 50.0f, 50.0f);
    float radius = 150.0f;
    float azimuth = 0.0f;
    float elevation = M_PI / 2.0f;
    float orbit_speed = 0.005f;
    float zoom_speed = 5.0f;
    float pan_speed = 1.0f;
    bool dragging = false;
    double lastX = 0.0f, lastY = 0.0f;

    glm::vec3 getPosition() const {
        float clampedElevation = glm::clamp(elevation, 0.01f, float(M_PI) - 0.01f);
        return glm::vec3(
            radius * sin(clampedElevation) * sin(azimuth), // Intercambiado para que el eje Y apunte arriba de forma natural
            radius * cos(clampedElevation),
            radius * sin(clampedElevation) * cos(azimuth)
        ) + target;
    }

    glm::mat4 getViewMatrix() const {
        glm::vec3 pos = getPosition();
        // glm::lookAt(Posición Cámara, A dónde mira, Cuál eje es "arriba")
        return glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void processMouseMovement(double x, double y) {
        float dx = float(x - lastX);
        float dy = float(y - lastY);
        if (dragging) {
            azimuth -= dx * orbit_speed; // Cambiado de signo para que el giro siga al cursor de forma intuitiva
            elevation -= dy * orbit_speed;
            elevation = glm::clamp(elevation, 0.01f, float(M_PI) - 0.01f); // Evita que la cámara se invierta en los polos
        }
        lastX = x;
        lastY = y;
    }

    void processMouseButton(int button, int action, GLFWwindow* win) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                dragging = true;
                glfwGetCursorPos(win, &lastX, &lastY);
            } else if (action == GLFW_RELEASE) {
                dragging = false;
            }
        }
    }

    void processScroll(double yoffset) {
        radius -= float(yoffset) * zoom_speed;
        if (radius < 0.5f) radius = 0.5f; // Límite de zoom para no atravesar los modelos
    }

    void processPan(float xoffset, float yoffset) {
        glm::vec3 pos = getPosition();
        glm::vec3 forward = glm::normalize(target-pos);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::cross(right, forward);

        target += right * xoffset * pan_speed;
        target += up * yoffset * pan_speed;
    }
};

OrbitCamera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    camera.processMouseButton(button,action,window);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.processMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processScroll(yoffset);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Simulacion 3D - Octree", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);

    Shader mainShader("shader.vs", "shader.fs");

    Cube cuboMolde(glm::vec3(0.0f), 1.0f);
    Octree octree(0, 0, 0, 100, 100, 100);
    octree.insert(20, 30, 40);
    octree.insert(80, 10, 50);

    globalOctree = &octree;
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processPan(0.0f,  1.0f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processPan(0.0f,  -1.0f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processPan(1.0f,  0.0f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processPan(-1.0f,  0.0f);

        glClearColor(0.15f, 0.16f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mainShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 1000.0f);

        glm::mat4 view = camera.getViewMatrix();
        mainShader.setMat4("view", glm::value_ptr(view));

        mainShader.setMat4("view", glm::value_ptr(view));
        mainShader.setMat4("projection", glm::value_ptr(projection));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_DEPTH_TEST);

        octree.draw(mainShader, cuboMolde);

        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteProgram(mainShader.ID);
    glfwTerminate();
    return 0;
}

