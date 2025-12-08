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

/*
*
   变换-旋转
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const float speedBase = 2.5f;

class Camera {
  public:
    glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_screenWidth = 4.0f;
    float m_screenHeight = 3.0f;

    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projection = glm::mat4(1.0f);

    bool firstMouse;
    float m_lastX, m_lastY;

    float m_fov = 45.0f;
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

  public:
    Camera(float screenWidth, float screenHeight)
        : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
    }

    void toForward(float delta);
    void toBack(float delta);
    void toLeft(float delta);
    void toRight(float delta);

    glm::mat4 &getView();
    glm::mat4 &getProjection();

    void frontMove(float xpos, float ypos);
    void frontScroll(float xoffset, float yoffset);
};

void Camera::toForward(float delta) {
  float cameraSpeed = speedBase * delta; //
  m_cameraPos += cameraSpeed * m_cameraFront;
}
void Camera::toBack(float delta) {
  float cameraSpeed = speedBase * delta; //
  m_cameraPos -= cameraSpeed * m_cameraFront;
}
void Camera::toLeft(float delta) {
  float cameraSpeed = speedBase * delta; //
  m_cameraPos -=
      glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
}
void Camera::toRight(float delta) {
  float cameraSpeed = speedBase * delta; //
  m_cameraPos +=
      glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
}

glm::mat4& Camera::getView() {
  m_view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
  return m_view;
}
glm::mat4& Camera::getProjection() {
  m_projection = glm::perspective(glm::radians(m_fov), m_screenWidth / m_screenHeight,
                                0.1f, 100.0f);
  return m_projection;
}

void Camera::frontMove(float xpos, float ypos) {
  // 这个bool变量初始时是设定为true的
  if (!firstMouse) {
      m_lastX = xpos;
      m_lastY = ypos;
      firstMouse = true;
  }
  float xoffset = xpos - m_lastX;
  // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
  float yoffset = m_lastY - ypos;
  m_lastX = xpos;
  m_lastY = ypos;

  float sensitivity = 0.05f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  m_yaw += xoffset;
  m_pitch = m_pitch + yoffset;
  if (m_pitch > 89.0f) {
      m_pitch = 89.0f;
  }
  if (m_pitch < -89.0f) {
      m_pitch = -89.0f;
  }

  glm::vec3 front;
  front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
  front.y = sin(glm::radians(m_pitch));
  front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
  m_cameraFront = glm::normalize(front);
}

void Camera::frontScroll(float xoffset, float yoffset) {
    m_fov -= yoffset;
    if (m_fov <= 1.0f) m_fov = 1.0f;
    if (m_fov >= 60.0f) m_fov = 60.0f;
    std::cout << "yoffset:" << yoffset << ", fov:" << m_fov << std::endl;
}

Camera camera = Camera(1080, 720);

float deltaTime = 0.0f;     // 当前帧与上一帧的时间差
float lastFrameTime = 0.0f; // 上一帧的时间

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

    GLFWwindow* window = glfwCreateWindow(1080, 720, "hello shader example1", NULL, NULL);
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

    common::Shader shader("shader/camera-01.vert", "shader/camera-01.frag");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

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

    glEnable(GL_DEPTH_TEST);
    // 在调用这个函数之后，无论我们怎么去移动鼠标，光标都不会显示了，它也不会离开窗口。对于FPS摄像机系统来说非常完美。
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f),

    };


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

        texture1.use();
        texture2.use();

        glm::mat4& view = camera.getView();
        glm::mat4& projection = camera.getProjection();

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        for (unsigned int i=0; i<10; i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 0.0f;
            if (i%3==0) {
                angle = timeValue*70.0f;
            } else {
                angle = 20.0f * i;
            }
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    shader.close();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    camera.frontMove(static_cast<float>(xpos), static_cast<float>(ypos));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.frontScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.toForward(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.toBack(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.toLeft(deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.toRight(deltaTime);
    }
}
