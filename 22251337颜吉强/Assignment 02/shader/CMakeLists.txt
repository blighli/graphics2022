cmake_minimum_required(VERSION 3.23)
project(shader)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

link_directories("D:/OpenGL/libs")
include_directories("D:/OpenGL/include")

add_executable(shader main.cpp src/glad.c shader.h stb_image.h camera.h)

target_link_libraries(shader opengl32.lib glfw3.lib)