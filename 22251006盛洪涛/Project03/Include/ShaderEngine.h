#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
enum shaderType {
	vertexShader,
	fragmentShader
};
class Shader{
public:
	int type;
	std::string shaderString;
	unsigned int shaderID;
	Shader(const char* path,int t):type(t){
		std::ifstream in;
		shaderID = 0;
		in.open(path);
		std::stringstream shaderStream;
		shaderStream << in.rdbuf();
		shaderString = shaderStream.str();
	}
	Shader() {

	}
	bool Compile() {
		int success;
		char infoLog[512];
		const char* cShaderString = shaderString.c_str();
		switch (type) {
			case 0:
				shaderID = glCreateShader(GL_VERTEX_SHADER);
				break;
			case 1:
				shaderID = glCreateShader(GL_FRAGMENT_SHADER);
				break;
		}
		
		glShaderSource(shaderID, 1, &cShaderString, NULL);
		glCompileShader(shaderID);
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
		return success;
	}
	void Apply(int programID) {
		glAttachShader(programID, shaderID);
	}
};
