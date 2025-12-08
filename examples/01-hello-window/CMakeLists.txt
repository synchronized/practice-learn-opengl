
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME TARGET_NAME_MAIN)
set(EXAMPLES_BINARY_DIR "${PROJECT_BINARY_DIR}/examples/${TARGET_NAME_MAIN}")

set(EXAMPLES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB files ${CMAKE_CURRENT_SOURCE_DIR}/main-*.cpp)
foreach(file ${files})
    cmake_path(GET file STEM stem)
    string(REGEX REPLACE "^main-(.+)$" "\\1" postfix_name ${stem})

    set(TARGET_NAME "${TARGET_NAME_MAIN}_${postfix_name}")
    add_executable(${TARGET_NAME} ${file})
    target_include_directories(${TARGET_NAME} PRIVATE "${EXAMPLES_SOURCE_DIR}")
    target_link_libraries(${TARGET_NAME} PRIVATE glad::glad)
    target_link_libraries(${TARGET_NAME} PRIVATE glfw)
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples/${TARGET_NAME_MAIN}")
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_BINARY_DIR}")

    add_dependencies(examples ${TARGET_NAME})
endforeach()