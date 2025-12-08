
set(TARGET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCE_FILES ${TARGET_SOURCE_DIR}/*.cpp)

set(TARGET_NAME common)
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PUBLIC "${TARGET_SOURCE_DIR}")
target_link_libraries(${TARGET_NAME} PUBLIC glad::glad)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)
target_link_libraries(${TARGET_NAME} PUBLIC stb)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "examples/${TARGET_NAME}")

add_dependencies(examples ${TARGET_NAME})
