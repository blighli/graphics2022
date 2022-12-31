#pragma once
#include <iostream>
#include <glad/glad.h>
#include "StringUtil.h"

//着色器
class Shader
{
public:
	Shader(std::istream* source, GLuint type);
	~Shader();
	//获取输入流
	std::istream* getSource() const;
	//获取类型
	GLuint getType() const;
	//获取着色器id
	GLuint getId() const;
	//编译着色器
	bool compile();

private:
	//输入流
	std::istream* source;
	//类型
	const GLuint type;
	//id（由OpenGL生成，在编译前不可用）
	GLuint id;
};
