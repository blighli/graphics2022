#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

GLuint loadShader(const std::string& vertexPath, const std::string& fragmentPath);
#endif