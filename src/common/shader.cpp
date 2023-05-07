
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

#include "common/shader.h"

namespace common {


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexShaderSource, fragmentShaderSource;
    std::ifstream vertexShaderFile, fragmentShaderFile;
    vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexShaderSource = vertexShaderStream.str();
        fragmentShaderSource = fragmentShaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }

    const char* vShaderCode = vertexShaderSource.c_str();
    const char* fShaderCode = fragmentShaderSource.c_str();

    unsigned int vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::COMPILATION::FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(ID);
}
void Shader::setBool(const std::string& name, bool v1) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)v1);
}
void Shader::setInt(const std::string& name, int v1) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), v1);
}
void Shader::setFloat(const std::string& name, float v1) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), v1);
}
void Shader::setVec2(const std::string& name, float v1, float v2) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
}

void Shader::setMat4(const std::string& name, glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::close() {
    glDeleteProgram(ID);
}

}
