
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

add_subdirectory(examples)
