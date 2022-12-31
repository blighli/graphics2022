/*################################################################
##  文件：Shader.h
##  文件说明： Shader类的头文件。Shader类的功能包括创建包括Vertex Shader
和Fragment Shader的Shader Program、使用Shader Program和为Shader中的
uniform赋值。
##################################################################*/

#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Shader
{
public:
	// 程序ID
	unsigned int programID;
	// 构造函数，读取Shader源码并构建Shader Program
	Shader(std::string const vertexPath, std::string const fragmentPath);
	Shader();
	// 使用Shader Program
	void use();
	// uniform工具函数
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec3(const std::string &name, float value1, float value2, float value3) const;
	void setMat4(const std::string &name, const glm::mat4 &mat);
};

#endif