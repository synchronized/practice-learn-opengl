
cmake_minimum_required(VERSION 3.23)

project(practice-learn-opengl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

#glad provides CMake targets:
find_package(glad CONFIG REQUIRED)

#glfw3 provides CMake targets:
find_package(glfw3 CONFIG REQUIRED)

#glm provides CMake targets:
find_package(glm CONFIG REQUIRED)


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB files src/*.cpp)
foreach(file ${files})
    get_filename_component(name ${file} NAME_WLE)
    add_executable(${name})
    target_sources(${name} PRIVATE ${file})
    target_link_libraries(${name} PRIVATE glad::glad)
    target_link_libraries(${name} PRIVATE glfw)
    target_link_libraries(${name} PRIVATE glm::glm)
endforeach()
