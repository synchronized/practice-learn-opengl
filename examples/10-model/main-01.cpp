#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "common/shader.h"
#include "common/camera.h"
#include "common/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

common::Camera camera = common::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
bool leftButtonDown = false;
bool altOrbit = false;
glm::vec3 orbitPivot = glm::vec3(0.0f);
float orbitDistance = 3.0f;
float orbitYaw = -90.0f;   // degrees
float orbitPitch = 0.0f;   // degrees

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
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods){
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                leftButtonDown = true;
                firstMouse = true; // 重置初始位置，避免跳变
                altOrbit = (mods & GLFW_MOD_ALT);
                if (altOrbit) {
                    // 以屏幕中心射线与 y=0 平面的交点作为 pivot，若无交点则向前方取默认距离
                    glm::vec3 rayOrigin = camera.m_Position;
                    glm::vec3 rayDir = glm::normalize(camera.m_Front);
                    float denom = rayDir.y;
                    if (std::fabs(denom) > 1e-4f) {
                        float t = -rayOrigin.y / denom; // 平面 y=0
                        if (t > 0.0f) {
                            orbitPivot = rayOrigin + t * rayDir;
                        } else {
                            orbitPivot = rayOrigin + rayDir * 3.0f;
                        }
                    } else {
                        orbitPivot = rayOrigin + rayDir * 3.0f;
                    }
                    orbitDistance = glm::length(rayOrigin - orbitPivot);
                    if (orbitDistance < 0.1f) orbitDistance = 0.1f;
                    glm::vec3 offset = rayOrigin - orbitPivot;
                    orbitYaw = glm::degrees(std::atan2(offset.z, offset.x));
                    orbitPitch = glm::degrees(std::asin(offset.y / orbitDistance));
                }
            } else if (action == GLFW_RELEASE) {
                leftButtonDown = false;
                altOrbit = false;
            }
        }
    });
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    common::Shader model_shader("shader/01-material.vert", "shader/01-material.frag");
    common::Model helmet_model("assets/models/FlightHelmet/glTF/FlightHelmet.gltf");

    while(!glfwWindowShouldClose(window)) {
        float currFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currFrameTime - lastFrameTime;
        lastFrameTime = currFrameTime;

        processInput(window); // 判断按键按下，是否关闭窗口
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.m_Zoom), 
            screenWidth / screenHeight, 0.1f, 100.0f);

        model_shader.use();
        model_shader.setMat4("view", view);
        model_shader.setMat4("projection", projection);
        model_shader.setFloat("material.shininess", 32.0f);
        model_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        model_shader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        model_shader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        model_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model_shader.setMat4("model", model);
        helmet_model.Draw(model_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    model_shader.close();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (!leftButtonDown) {
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    if (altOrbit) {
        float sensitivity = 0.1f;
        orbitYaw   += xoffset * sensitivity;
        // 反转竖直方向，使鼠标上推视角上抬更符合直觉
        orbitPitch -= yoffset * sensitivity;
        if (orbitPitch > 89.0f) orbitPitch = 89.0f;
        if (orbitPitch < -89.0f) orbitPitch = -89.0f;

        float yawRad = glm::radians(orbitYaw);
        float pitchRad = glm::radians(orbitPitch);
        glm::vec3 newPos;
        newPos.x = orbitPivot.x + orbitDistance * cos(pitchRad) * cos(yawRad);
        newPos.y = orbitPivot.y + orbitDistance * sin(pitchRad);
        newPos.z = orbitPivot.z + orbitDistance * cos(pitchRad) * sin(yawRad);

        camera.m_Position = newPos;
        camera.m_Front = glm::normalize(orbitPivot - camera.m_Position);
        camera.m_Right = glm::normalize(glm::cross(camera.m_Front, camera.m_WorldUp));
        camera.m_Up = glm::normalize(glm::cross(camera.m_Right, camera.m_Front));
    } else {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
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
