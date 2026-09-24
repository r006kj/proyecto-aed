//
// Created by leona on 24/09/2026.
//

#ifndef PROYECTO1_SHADER_H
#define PROYECTO1_SHADER_H


#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setMat4(const std::string &name, const float* matrixPtr) const;
};


#endif //PROYECTO1_SHADER_H