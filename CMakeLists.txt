cmake_minimum_required(VERSION 4.0)
project(Proyecto1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto1 main.cpp glad.c
        Shader.h
        Shader.cpp
        Cube.h
        Cube.cpp)

target_link_libraries(Proyecto1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a opengl32)
target_include_directories(Proyecto1 PRIVATE include)

configure_file(shader.vs ${CMAKE_CURRENT_BINARY_DIR}/shader.vs COPYONLY)
configure_file(shader.fs ${CMAKE_CURRENT_BINARY_DIR}/shader.fs COPYONLY)
