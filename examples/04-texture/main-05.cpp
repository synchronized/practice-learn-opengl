#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "common/shader.h"
#include "common/texture.h"

/**
   1.调整两个纹理可见性
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

    common::Shader shader("shader/texture-05.vert", "shader/texture-05.frag");

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int EBO, VBO, VAO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    common::Texture texture1(GL_TEXTURE0);
    texture1.setWrapS(GL_CLAMP_TO_EDGE)
            .setWrapT(GL_CLAMP_TO_EDGE);
    if (texture1.open("assets/texture/container.jpg", GL_RGB) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    common::Texture texture2(GL_TEXTURE1);
    texture2.setFormat(GL_RGBA);
    if (texture2.open("assets/texture/awesomeface.png", GL_RGBA) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    shader.use();
    shader.setInt("ourTexture1", texture1.getIndex());
    shader.setInt("ourTexture2", texture2.getIndex());


    while(!glfwWindowShouldClose(window)) {
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float minOffset = sin(timeValue)/2+0.5;

        texture1.use();
        texture2.use();

        shader.use();
        shader.setFloat("mixOffset", minOffset);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    shader.close();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
