#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <cmath>

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

void Shader::close() {
    glDeleteProgram(ID);
}

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

    Shader shader("shader/shader-hello2.vertexshader", "shader/shader-hello2.fragmentshader");
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
