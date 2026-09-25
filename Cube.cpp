//
// Created by leona on 24/09/2026.
//

#include "Cube.h"

#include "GLFW/glfw3.h"

Cube::Cube(glm::vec3 center, float size, glm::vec3 color)
    : center(center), size(size), color(color), VAO(0), VBO(0), EBO(0) {
    setupMesh();
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::setupMesh() {
    float h = size / 2.0f; // Mitad del tamaño para un cubo unitario

    float vertices[] = {
        // Solo posiciones locales (X, Y, Z) - 3 floats por vértice
        -h, -h,  h,   // 0
         h, -h,  h,   // 1
         h,  h,  h,   // 2
        -h,  h,  h,   // 3
        -h, -h, -h,   // 4
         h, -h, -h,   // 5
         h,  h, -h,   // 6
        -h,  h, -h    // 7
    };

    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0, // Frente
        1, 5, 6,  6, 2, 1, // Derecha
        4, 0, 3,  3, 7, 4, // Izquierda
        4, 5, 1,  1, 0, 4, // Abajo
        3, 2, 6,  6, 7, 3, // Arriba
        5, 4, 7,  7, 6, 5  // Atrás
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. Enlazar el VAO principal
    glBindVertexArray(VAO);

    // 2. Vincular y llenar el VBO de posiciones
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. Vincular y llenar el EBO de índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. CORRECCIÓN DEL STRIDE: Cambiado de 6 a 3 * sizeof(float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 5. DESVINCULAR EXCLUSIVAMENTE EL VAO
    // ¡Ojo! No debes desvincular el GL_ARRAY_BUFFER ni el EBO antes que el VAO,
    // o el VAO olvidará de dónde leer los datos de la GPU.
    glBindVertexArray(0);
}

void Cube::draw(const Shader& shader) const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,center);

    float time = (float)glfwGetTime();
    model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    shader.setMat4("model", glm::value_ptr(model));

    int colorLoc = glGetUniformLocation(shader.ID, "objectColor");
    glUniform3f(colorLoc, color.r, color.g, color.b);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}