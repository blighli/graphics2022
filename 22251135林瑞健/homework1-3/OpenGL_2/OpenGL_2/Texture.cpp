#include "Texture.h"
#include <SOIL.h>

Texture::Texture(std::string& source, GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint magFilter)
{
	this->source = source;
	this->wrapS = wrapS;
	this->wrapT = wrapT;
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}

Texture::~Texture()
{
}

std::string Texture::getSource() const
{
	return source;
}

GLuint Texture::getWrapS() const
{
	return wrapS;
}

GLuint Texture::getWrapT() const
{
	return wrapT;
}

GLuint Texture::getMinFilter() const
{
	return minFilter;
}

GLuint Texture::getMagFilter() const
{
	return magFilter;
}

GLuint Texture::getId() const
{
	return id;
}

void Texture::loadSource()
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	int width, height;
	unsigned char* image = SOIL_load_image(source.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}
