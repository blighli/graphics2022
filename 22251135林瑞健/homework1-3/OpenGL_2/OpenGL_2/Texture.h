#pragma once
#include <glad/glad.h>
#include <string>

//贴图
class Texture
{
public:
	Texture(std::string& source, GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint magFilter);
	~Texture();
	//获取贴图路径
	std::string getSource() const;
	//获取s方向超出贴图坐标范围的处理方式
	GLuint getWrapS() const;
	//获取t方向超出贴图坐标范围的处理方式
	GLuint getWrapT() const;
	//获取缩小贴图的处理方式
	GLuint getMinFilter() const;
	//获取放大贴图的处理方式
	GLuint getMagFilter() const;
	//获取id
	GLuint getId() const;
	//加载贴图
	void loadSource();

private:
	//贴图路径
	std::string source;
	//s方向超出贴图坐标范围的处理方式
	GLuint wrapS;
	//t方向超出贴图坐标范围的处理方式
	GLuint wrapT;
	//缩小贴图的处理方式
	GLuint minFilter;
	//放大贴图的处理方式
	GLuint magFilter;
	//id（由OpenGL生成，在编译前不可用）
	GLuint id;
};
