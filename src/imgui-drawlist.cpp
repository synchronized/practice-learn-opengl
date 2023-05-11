
#include <glad/glad.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <iostream>

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char* glsl_version = "#version 330";

int main() {
    glfwSetErrorCallback(error_callback);
    //初始化, 并且设置版本号
    if (!glfwInit()) {
        std::cout << "Failed to glfwInit" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    //加载OpenGL函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 bgcol = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImVec4 txtcol = ImVec4(1.0f, 0.55f, 0.60f, 1.00f);

    //渲染循环
    int vertexNum = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); //检查事件
        glClearColor(bgcol.x * bgcol.w, bgcol.y * bgcol.w, bgcol.z * bgcol.w, bgcol.w);
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window); // 判断按键按下，是否关闭窗口

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float currTime = static_cast<float>(glfwGetTime());

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Quit", "Alt+F4")) break;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if( ImGui::MenuItem("TODO") ){
                    //do something
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        vertexNum++;
        if (show_another_window)
        {
            ImGuiWindowFlags window_flags = 0; //显示菜单
            window_flags |= ImGuiWindowFlags_MenuBar;
            ImGui::Begin("Another Window", &show_another_window, window_flags);
            const ImVec2 mousePos = io.MousePos;
            ImGui::Text("mousePos.x:%s, mousePos.y:%d", mousePos.x, mousePos.y);

            ImDrawList* drawList = ImGui::GetForegroundDrawList();
            drawList->AddRectFilled(ImVec2(0.0f, 0.0f), mousePos, IM_COL32(25, 125, 225, 255));
            drawList->AddRect(ImVec2(0.0f, 0.0f), ImVec2(100.0f, 100.0f), IM_COL32(225, 125, 25, 255));
            drawList->AddQuad(ImVec2(1.0f, 101.0f), ImVec2(101.0f, 151.0f), ImVec2(101.0f, 201.0f), ImVec2(1.0f, 201.0f), IM_COL32(225, 125, 25, 255));

            drawList->AddCircle(ImVec2(100.0f, 301.0f), 50.0f, IM_COL32(225, 125, 25, 255), vertexNum/5);

            drawList->AddNgon(ImVec2(100.0f, 451.0f), 50.0f, IM_COL32(225, 125, 25, 255), vertexNum/5);

            drawList->PathArcTo(ImVec2(100.0f, 501.0f), 50.0f, 0.0f, IM_PI, 16);
            drawList->PathStroke(IM_COL32(225, 125, 25, 255), 0, 1.0f);
            drawList->PathArcToFast(ImVec2(100.0f, 651.0f), 50.0f, 0.0f, 10.0f);
            drawList->PathStroke(IM_COL32(225, 125, 25, 255), 0, 1.0f);


            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); //交换缓冲区
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
