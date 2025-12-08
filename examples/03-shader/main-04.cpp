#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>

#include "common/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//-----------------------------------------------
class Line {
  public:
    unsigned int VBO, VAO;

    Line(float p1x, float p1y, float p2x, float p2y);

    void Draw() const;
    void close();
};

Line::Line(float p1x, float p1y, float p2x, float p2y) {
    float vertices[] = {p1x, p1y, 0.0f, p2x, p2y, 0.0f};

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Line::Draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void Line::close() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

//-----------------------------------------------
class Triangle {
  public:
    unsigned int EBO, VBO, VAO;
    //std::array<float, 18> vertices;
    //std::array<unsigned int, 3> indexes;

    Triangle();

    void close();
    void Draw() const;
};

Triangle::Triangle() {
    float vertices[] = {
        -0.3f, -0.3f, 0.0f,   1.0f, 0.0f, 0.0f,
        0.3f, -0.3f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.0f, 0.3f, 0.0f,     0.0f, 0.0f, 1.0f,
    };

    unsigned int indexes[] = {
        0, 1, 2,
    };

    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
}

void Triangle::close() {
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Triangle::Draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
}

//-----------------------------------------------
int main() {
    if (!glfwInit()) {
        std::cout << "glfwInit Failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "hello shader example1", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to glfwCreateWindow()" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to gladLoadGLLoader()" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    common::Shader shader("shader/shader-04.vert", "shader/shader-04.frag");
    Line line1(-0.5f, 0.5f, 0.5f, 0.5f);
    Line line2(-0.5f, -0.5f, 0.5f, -0.5f);
    Line line3(-0.5f, 0.5f, -0.5f, -0.5f);
    Line line4(0.5f, 0.5f, 0.5f, -0.5f);

    Triangle triangle;


    while(!glfwWindowShouldClose(window)) {
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float xoffset = sin(timeValue)/2;
        float yoffset = cos(timeValue)/2;

        shader.use();
        shader.setVec2("offset", xoffset, yoffset);

        line1.Draw();
        line2.Draw();
        line3.Draw();
        line4.Draw();

        triangle.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    line1.close();
    line2.close();
    line3.close();
    line4.close();

    triangle.close();
    shader.close();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
