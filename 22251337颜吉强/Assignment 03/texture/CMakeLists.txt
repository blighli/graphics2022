cmake_minimum_required(VERSION 3.23)
SET(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(shader)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(assimp CONFIG REQUIRED)


link_directories("D:/OpenGL/libs")
include_directories("D:/OpenGL/include")

add_executable(shader main.cpp src/glad.c shader.h stb_image.h camera.h mesh.h model.h)
target_link_libraries(shader opengl32.lib glfw3.lib assimp::assimp)