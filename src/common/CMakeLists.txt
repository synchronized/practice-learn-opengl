

file(GLOB files *.cpp)

set(name common)

add_library(${name} STATIC)
target_sources(${name} PRIVATE ${files})
target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(${name} PRIVATE glad::glad)
target_link_libraries(${name} PRIVATE glfw)
target_link_libraries(${name} PRIVATE glm::glm)
target_link_libraries(${name} PRIVATE stb)
target_link_libraries(${name} PRIVATE imgui::imgui)
