#include <glad/glad.h>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Texture.h"
#include "RenderUtil.h"

Texture::Data::~Data()
{
	glDeleteTextures(1, &id);
}

Texture::Texture()
	: data()
{
}

Texture::Texture(std::shared_ptr<Texture::Data> data, TextureType type)
	: data(data), type(type)
{
}

TextureType Texture::GetType() const
{
	return type;
}

void Texture::SetType(TextureType type)
{
	this->type = type;
}

void Texture::Bind() const
{
	if (data == nullptr)
		return;

	if (type == TextureType::Cubemap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, data->id);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, data->id);
	}
}

GLuint Texture::GetID() const
{
	return data->id;
}

Texture::Data TextureLoader::LoadFromFile(const std::string &imageLocation)
{
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glCheckError();

	int width, height, nrComponents;
	unsigned char *data = stbi_load(imageLocation.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glCheckError();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glCheckError();

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else
	{
		printf("Texture failed to load at path: %s \n", imageLocation.c_str());
		stbi_image_free(data);
	}

	return Texture::Data(textureID);
}

Texture TextureLoader::LoadFromFile(TextureType type, const std::string &imageLocation)
{
	return Texture(std::make_shared<Texture::Data>(LoadFromFile(imageLocation)), type);
}

Texture TextureLoader::LoadCubemap(const std::string &directory, const std::vector<std::string> &filename)
{
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glCheckError();

	assert(filename.size() == 6);

	std::string::const_iterator it = directory.end();
	it--;
	std::string baseDir = (*it == '/') ? directory : directory + "/";

	for (unsigned int i = 0; i != filename.size(); i++)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load((baseDir + filename[i]).c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glCheckError();

			stbi_image_free(data);
		}
		else
		{
			printf("Texture failed to load at path: %s \n", (baseDir + filename[i]).c_str());
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glCheckError();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return Texture(std::make_shared<Texture::Data>(id), TextureType::Cubemap);
}

Texture TextureLoader::LoadHDR(const std::string &filename)
{
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glCheckError();

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glCheckError();

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	else
	{
		printf("Texture failed to load at path: %s \n", filename.c_str());
	}

	stbi_set_flip_vertically_on_load(false);

	return Texture(std::make_shared<Texture::Data>(textureID), TextureType::HDR);
}