
#include <glad/glad.h>

#include "imgui.h"
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

    struct tButton{
        bool CheckBox = false;
        int CheckBoxFlagI = 0;
        int Radio = 0;
    } tb;

    //渲染循环
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

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGuiWindowFlags window_flags = 0; //显示菜单
            window_flags |= ImGuiWindowFlags_MenuBar;
            ImGui::Begin("Another Window", &show_another_window, window_flags);
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Examples")) {
                    if (ImGui::MenuItem("Main menu bar")) {
                        std::cout << "menuItem: Main menu bar" << std::endl;
                    }
                    if (ImGui::MenuItem("Console")) {
                        std::cout << "menuItem: Console" << std::endl;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;

            //Text
            ImGui::Text("Text: normal");
            ImGui::TextColored(txtcol, "TextColored %d", 123);
            ImGui::TextDisabled("TextDisabled %d", 123);
            ImGui::TextWrapped("TextWrapped %d", 123);
            ImGui::LabelText("label1", "LabelText");
            ImGui::BulletText("BulletText");

            //Button
            //ImGui::Button("Button1", ImVec2(20.0f, 20.0f));
            ImGui::Button("Button1");
            ImGui::SmallButton("SmallButton1");
            ImGui::InvisibleButton("InvisibleButton1", ImVec2(20.0f, 50.0f));
            ImGui::ArrowButton("ArrowButton1", ImGuiDir_Left);
            ImGui::ArrowButton("##right", ImGuiDir_Right);
            ImGui::Checkbox("Checkbox1", &tb.CheckBox);
            ImGui::CheckboxFlags("Checkbox2", &tb.CheckBoxFlagI, 1);
            ImGui::CheckboxFlags("Checkbox3", &tb.CheckBoxFlagI, 2);
            ImGui::CheckboxFlags("Checkbox4", &tb.CheckBoxFlagI, 4);
            ImGui::RadioButton("RadioButton1", &tb.Radio, 1);
            ImGui::RadioButton("RadioButton2", &tb.Radio, 2);
            ImGui::RadioButton("RadioButton3", &tb.Radio, 3);
            if (currTime > 1.0f) {
                currTime -= 1.0f;
            }
            ImGui::ProgressBar(currTime, ImVec2(0.0f, 0.0f));

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
