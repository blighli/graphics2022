
#include "wrapperShader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define LOG_LEN   512

wrapperShader::wrapperShader()
:m_shaderProgram(0)
{
}

wrapperShader::~wrapperShader(){}

wrapperShader wrapperShader::createWithFilePath(const char *vPath, const char *fPath)
{
	std::ifstream v, f;
	v.open(vPath);
	f.open(fPath);
	

	std::stringstream vStrS, fStrS;
	vStrS << v.rdbuf();
	fStrS << f.rdbuf();
	v.close();
	f.close();

	std::string str_v, str_f;
	str_v=vStrS.str();
	str_f =fStrS.str();
	
	return wrapperShader::create(str_v.c_str(), str_f.c_str());
}

wrapperShader wrapperShader::create(const char *vSource, const char *fSource)
{
	wrapperShader ret =wrapperShader();
	ret.init(vSource, fSource);
	return ret;
}

bool wrapperShader::init(const char *vSource, const char *fSource)
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vSource, nullptr);
	glCompileShader(vShader);

	GLint suc;
	char infoLog[LOG_LEN] = { 0 };

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &suc);
	if (!suc)
	{
		glGetShaderInfoLog(vShader, LOG_LEN, nullptr, infoLog);
		std::cout << "vertex shader compile failed:" << infoLog << std::endl;
		return false;
	}

	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fSource, nullptr);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &suc);
	if (!suc)
	{
		glGetShaderInfoLog(fShader, LOG_LEN, nullptr, infoLog);
		std::cout << "fragment shader compile failed:" << infoLog << std::endl;
		return false;
	}

	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vShader);
	glAttachShader(m_shaderProgram, fShader);
	glLinkProgram(m_shaderProgram);

	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &suc);
	if (!suc)
	{
		glGetProgramInfoLog(m_shaderProgram, LOG_LEN, nullptr, infoLog);
		std::cout << "link program failed:" << infoLog << std::endl;

		m_shaderProgram = 0;
		return false;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return true;
}

void wrapperShader::useShaderProgram()
{
	if (m_shaderProgram!=0)
	{
		glUseProgram(m_shaderProgram);
	}
}

void wrapperShader::setVec3(const char *name, float x, float y, float z)
{
	unsigned int transformLoc = glGetUniformLocation(m_shaderProgram, name);
	if (transformLoc>=0)
	{
		GLfloat vec[3];
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		glUniform3fv(transformLoc, 1, &vec[0]);
	}
	else{
		std::cout << "setVec3:not find " << name << " location" << std::endl;
	}
}

void wrapperShader::setVec3(const char *name, glm::vec3 parm)
{
	this->setVec3(name, parm.x, parm.y, parm.z);
}

void wrapperShader::setMat4fv(const char *name, glm::mat4 parm)
{
	unsigned int transformLoc = glGetUniformLocation(m_shaderProgram, name);
	if (transformLoc>=0)
	{
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(parm));
	}
	else{
		std::cout << "setMat4fv:not find " << name << " location" << std::endl;
	}
}

void wrapperShader::set1i(const char *name, int parm)
{
	unsigned int transformLoc = glGetUniformLocation(m_shaderProgram, name);
	if (transformLoc >= 0)
	{
		glUniform1i(transformLoc, parm);
	}
}

void wrapperShader::set1f(const char *name, float parm)
{
	unsigned int transformLoc = glGetUniformLocation(m_shaderProgram, name);
	if (transformLoc >= 0)
	{
		glUniform1f(transformLoc, parm);
	}
}