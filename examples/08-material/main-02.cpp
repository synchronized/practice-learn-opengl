#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

#include "common/texture.h"
#include "common/shader.h"
#include "common/camera.h"

/*
*
   变换-旋转
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

common::Camera camera = common::Camera(glm::vec3(0.0f, 1.0f, 4.0f));

bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;

float deltaTime = 0.0f;     // 当前帧与上一帧的时间差
float lastFrameTime = 0.0f; // 上一帧的时间

float screenWidth = 1080;
float screenHeight = 720;

//-----------------------------------------------
int main() {
    if (!glfwInit()) {
        std::cout << "glfwInit Failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL lighting", NULL, NULL);
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 在调用这个函数之后，无论我们怎么去移动鼠标，光标都不会显示了，它也不会离开窗口。对于FPS摄像机系统来说非常完美。
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int VBO, containerVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &containerVAO);

    glBindVertexArray(containerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为容器(物体)的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性指针(仅设置灯的顶点数据)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    //glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Light attributes
    //glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightPos(0.0f, 0.0f, -3.0f);

    common::Shader lamp_shader("shader/02-lamp.vert", "shader/02-lamp.frag");
    common::Shader box1_shader("shader/02-material.vert", "shader/02-material.frag");
    common::Shader box2_shader("shader/02-material.vert", "shader/02-material-reverse-specular.frag"); //反转高光
    common::Shader box4_shader("shader/02-material.vert", "shader/02-material-emission.frag"); //自发光

    common::Texture box1_diffuse_tex(GL_TEXTURE0);
    if (box1_diffuse_tex.open("assets/texture/container2.png", GL_RGB) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    common::Texture box1_specular_tex(GL_TEXTURE1);
    if (box1_specular_tex.open("assets/texture/container2_specular.png", GL_RGB) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    //带颜色的高光
    common::Texture box3_specular_tex(GL_TEXTURE1);
    if (box3_specular_tex.open("assets/texture/lighting_maps_specular_color.png", GL_RGB) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    //自发光
    common::Texture box4_emission_tex(GL_TEXTURE1);
    if (box4_emission_tex.open("assets/texture/matrix.jpg", GL_RGB) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    while(!glfwWindowShouldClose(window)) {
        float currFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currFrameTime - lastFrameTime;
        lastFrameTime = currFrameTime;

        processInput(window); // 判断按键按下，是否关闭窗口
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = currFrameTime;
        float minOffset = sin(timeValue)/2+0.5;

        // change the light's position values over time (can be done anywhere in the render loop actually, but try to do it at least before using the light source positions)
        //lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        //lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.m_Zoom), 
            screenWidth / screenHeight, 0.1f, 100.0f);

        glm::mat4 lampModel(1.0f);
        lampModel = glm::translate(lampModel, lightPos);
        lampModel = glm::scale(lampModel, glm::vec3(0.2f)); // Make it a smaller cube

        lamp_shader.use();
        lamp_shader.setMat4("view", view);
        lamp_shader.setMat4("projection", projection);
        lamp_shader.setMat4("model", lampModel);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));

        glm::vec3 diffuseColor = {0.5f, 0.5f, 0.5f};
        glm::vec3 ambientColor = {0.2f, 0.2f, 0.2f};

        // 第一个盒子，正常高光
        box1_shader.use();
        box1_shader.setMat4("view", view);
        box1_shader.setMat4("projection", projection);
        box1_shader.setMat4("model", model);

        box1_diffuse_tex.use();
        box1_specular_tex.use();
        box1_shader.setInt("material.diffuse", box1_diffuse_tex.getIndex());
        box1_shader.setInt("material.specular", box1_specular_tex.getIndex());
        box1_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        box1_shader.setFloat("material.shininess", 32.0f);
        box1_shader.setVec3("light.position", lightPos);
        box1_shader.setVec3("light.ambient", ambientColor);
        box1_shader.setVec3("light.diffuse", diffuseColor);
        box1_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // 第二个盒子，反转高光
        model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, -2.0f));
        box2_shader.use();
        box2_shader.setMat4("view", view);
        box2_shader.setMat4("projection", projection);
        box2_shader.setMat4("model", model);

        box1_diffuse_tex.use();
        box1_specular_tex.use();
        box2_shader.setInt("material.diffuse", box1_diffuse_tex.getIndex());
        box2_shader.setInt("material.specular", box1_specular_tex.getIndex());
        box2_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        box2_shader.setFloat("material.shininess", 32.0f);
        box2_shader.setVec3("light.position", lightPos);
        box2_shader.setVec3("light.ambient", ambientColor);
        box2_shader.setVec3("light.diffuse", diffuseColor);
        box2_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 第三个盒子，带颜色的高光
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.0f, -2.0f));
        box1_shader.use();
        box1_shader.setMat4("view", view);
        box1_shader.setMat4("projection", projection);
        box1_shader.setMat4("model", model);

        box1_diffuse_tex.use();
        box3_specular_tex.use();
        box1_shader.setInt("material.diffuse", box1_diffuse_tex.getIndex());
        box1_shader.setInt("material.specular", box3_specular_tex.getIndex());
        box1_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        box1_shader.setFloat("material.shininess", 32.0f);
        box1_shader.setVec3("light.position", lightPos);
        box1_shader.setVec3("light.ambient", ambientColor);
        box1_shader.setVec3("light.diffuse", diffuseColor);
        box1_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 第四个盒子，自发光贴图
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.0f, -4.0f));
        box4_shader.use();
        box4_shader.setMat4("view", view);
        box4_shader.setMat4("projection", projection);
        box4_shader.setMat4("model", model);

        box1_diffuse_tex.use();
        box1_specular_tex.use();
        box4_emission_tex.use();
        box4_shader.setInt("material.diffuse", box1_diffuse_tex.getIndex());
        box4_shader.setInt("material.specular", box1_specular_tex.getIndex());
        box4_shader.setInt("material.emission", box4_emission_tex.getIndex());
        box4_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        box4_shader.setFloat("material.shininess", 32.0f);
        box4_shader.setVec3("light.position", lightPos);
        box4_shader.setVec3("light.ambient", ambientColor);
        box4_shader.setVec3("light.diffuse", diffuseColor);
        box4_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &containerVAO);
    glDeleteVertexArrays(1, &lightVAO);

    lamp_shader.close();
    box1_shader.close();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(common::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(common::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(common::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(common::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.ProcessKeyboard(common::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.ProcessKeyboard(common::DOWN, deltaTime);
}
