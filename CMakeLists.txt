
cmake_minimum_required(VERSION 3.23)

project(practice-learn-opengl)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# By default, MSVC uses the GBK encoding.
# To use UTF-8 without a BOM header,
#     you need to add the following statement.
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(3rd)
add_subdirectory(src/common)

file(GLOB files src/*.cpp)
foreach(file ${files})
    get_filename_component(name ${file} NAME_WLE)
    add_executable(${name})
    target_sources(${name} PRIVATE ${file})
    target_include_directories(${name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(${name} PRIVATE glad::glad)
    target_link_libraries(${name} PRIVATE glfw)
    target_link_libraries(${name} PRIVATE glm::glm)
    target_link_libraries(${name} PRIVATE stb)
    target_link_libraries(${name} PRIVATE imgui::imgui)
    target_link_libraries(${name} PRIVATE common)
endforeach()
