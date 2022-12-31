#include "shader.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
void check_errors(GLuint id, const std::string& type) {
	GLint success;
	if (type == "SHADER") {
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLint logLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> infoLog((logLength > 1) ? logLength : 1);
			glGetShaderInfoLog(id, logLength, nullptr, infoLog.data());
			std::cerr << infoLog.data() << std::endl;
		}
	}
	else {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			GLint logLength;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> infoLog((logLength > 1) ? logLength : 1);
			glGetProgramInfoLog(id, logLength, nullptr, infoLog.data());
			std::cerr << infoLog.data() << std::endl;
		}
	}
}

GLuint loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexCode, fragmentCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;


	vertexShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fragmentShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try {
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}

	catch (std::ifstream::failure& e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
	}

	auto compileShader = [](GLuint& id, const std::string& code) {
		const char* shaderCode = code.c_str();
		glShaderSource(id, 1, &shaderCode, nullptr);
		glCompileShader(id);
		check_errors(id, "SHADER");
	};

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, vertexCode);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, fragmentCode);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	check_errors(shaderProgram, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}