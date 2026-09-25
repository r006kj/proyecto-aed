//
// Created by leona on 24/09/2026.
//

#ifndef PROYECTO1_CUBE_H
#define PROYECTO1_CUBE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class Cube {
private:
    unsigned int VAO, VBO, EBO;
    glm::vec3 center;
    float size;
    glm::vec3 color;

    void setupMesh();

public:
    Cube(glm::vec3 center, float size, glm::vec3 color = glm::vec3(0.3f, 0.6f, 0.9f));
    ~Cube();

    void draw(const Shader& shader) const;

    glm::vec3 getCenter() const { return center; }
    float getSize() const { return size; }
    unsigned int getVAO() const { return VAO; }
};


#endif //PROYECTO1_CUBE_H