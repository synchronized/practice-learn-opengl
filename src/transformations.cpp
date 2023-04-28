#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

/**
   变换
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Shader {
  public:
    //程序id
    unsigned int ID;

    //构造器
    Shader(const char* vertexPath, const char* fragmentPath);
    //使用/激活程序
    void use();
    //uniform工具函数
    void setBool(const std::string &name, bool v1) const;
    void setInt(const std::string &name, int v1) const;
    void setFloat(const std::string &name, float v1) const;
    void setVec2(const std::string &name, float v1, float v2) const;
    void setMat4f(const std::string &name, const float *value) const;
    void close();
};

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
void Shader::setBool(const std::string &name, bool v1) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)v1);
}
void Shader::setInt(const std::string &name, int v1) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), v1);
}
void Shader::setFloat(const std::string &name, float v1) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), v1);
}
void Shader::setVec2(const std::string &name, float v1, float v2) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
}

void Shader::setMat4f(const std::string &name, const float *value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}


void Shader::close() {
    glDeleteProgram(ID);
}

//-----------------------------------------------
class Texture {
  public:
    unsigned int m_textureId;
    unsigned int m_textureSlot;
    unsigned int m_format;
    unsigned int m_wrapType_s;
    unsigned int m_wrapType_t;
    unsigned int m_minFilter;
    unsigned int m_magFilter;

    Texture(unsigned int textureSlot): m_textureId(0),
                                       m_textureSlot(textureSlot),
                                       m_format(GL_RGB),
                                       m_wrapType_s(GL_REPEAT),
                                       m_wrapType_t(GL_REPEAT),
                                       m_minFilter(GL_LINEAR_MIPMAP_LINEAR),
                                       m_magFilter(GL_LINEAR) {}

    Texture& setFormat(unsigned int format) {m_format=format; return *this;}
    Texture& setWrapS(unsigned int wrapType) {m_wrapType_s = wrapType; return *this;}
    Texture& setWrapT(unsigned int wrapType) {m_wrapType_t = wrapType; return *this;}
    Texture& setMinFilter(unsigned int filterType) {m_minFilter = filterType; return *this;}
    Texture& setMagFilter(unsigned int filterType) {m_magFilter = filterType; return *this;}
    int getIndex() const {return m_textureSlot-GL_TEXTURE0;}

    int open(const std::string &texPath, unsigned int format);
    void close();
    void use();
};

int Texture::open(const std::string &texPath, unsigned int format) {
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapType_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapType_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL) {
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return 0;
}

void Texture::use() {
    glActiveTexture(m_textureSlot);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::close() {
    glDeleteTextures(1, &m_textureId);
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

    Shader shader("shader/transformations.vertexshader", "shader/transformations.fragmentshader");

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

    Texture texture1(GL_TEXTURE0);
    texture1.setWrapS(GL_CLAMP_TO_EDGE)
            .setWrapT(GL_CLAMP_TO_EDGE);
    if (texture1.open("assets/texture/container.jpg", GL_RGB) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    Texture texture2(GL_TEXTURE1);
    texture2.setFormat(GL_RGBA);
    if (texture2.open("assets/texture/awesomeface.png", GL_RGBA) != 0) {
        std::cout << "load texture image failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    shader.use();
    shader.setInt("ourTexture1", texture1.getIndex());
    shader.setInt("ourTexture2", texture2.getIndex());

    glm::mat4 trans(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

    shader.setMat4f("transform", glm::value_ptr(trans));

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

//-----------------------------------------------
int test1() {
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
    // 下面这行代码就需要改为:
    // glm::mat4 trans = glm::mat4(1.0f)
    // 之后将不再进行提示
    glm::mat4 trans(1.0f);
    std::cout << "trans :" << glm::to_string(trans) << std::endl;
    std::cout << "vec   :" << glm::to_string(vec) << std::endl;
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    std::cout << "trans :" << glm::to_string(trans) << std::endl;
    vec = trans * vec;
    std::cout << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z << std::endl;
    return 0;
}
