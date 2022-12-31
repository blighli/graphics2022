#include <glad/glad.h>
#include "StringUtil.h"
#include "Shader.h"

Shader::Shader(std::istream* source, GLuint type): source(source), type(type)
{
	
}

Shader::~Shader()
{
}

std::istream* Shader::getSource() const
{
	return source;
}


GLuint Shader::getType() const
{
	return type;
}

GLuint Shader::getId() const
{
	return id;
}

bool Shader::compile()
{
	id = glCreateShader(type);
	std::string content = StringUtil::readText(*(source));
	const char* shaderSource = content.c_str();
	glShaderSource(id, 1, &shaderSource, NULL);
	glCompileShader(id);
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cerr << "ERROR: Shader compilation failed" << '\n' << infoLog << '\n';
	}
	return !!success;
}
