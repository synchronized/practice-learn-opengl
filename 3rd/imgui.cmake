cmake_minimum_required(VERSION 3.20)
project(imgui)

set(root ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# 源文件列表
set(srcs 
    ${root}/imgui.cpp 
    ${root}/imgui_widgets.cpp 
    ${root}/imgui_tables.cpp 
    ${root}/imgui_draw.cpp
    ${root}/backends/imgui_impl_glfw.cpp
    ${root}/backends/imgui_impl_opengl3.cpp)

add_library(imgui ${srcs})
add_library(imgui::imgui ALIAS imgui)

# 设置包含目录 - 只包含头文件目录
target_include_directories(imgui 
    PUBLIC 
        $<BUILD_INTERFACE:${root}>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_INCLUDE_IMCONFIG_H)

target_link_libraries(imgui PRIVATE glfw)
#target_link_libraries(imgui PRIVATE Vulkan::Vulkan)
