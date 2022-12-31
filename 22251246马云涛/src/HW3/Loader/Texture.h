#pragma once
#include <memory>
#include <vector>
#include <string>

enum class TextureType
{
	Diffuse,
	Specular,
	Cubemap,
	HDR
};

typedef unsigned int GLuint;

class Texture
{
public:
	struct Data;

	Texture();
	Texture(std::shared_ptr<Texture::Data> data, TextureType type);

	TextureType GetType() const;
	void SetType(TextureType type);

	void Bind() const;
	GLuint GetID() const;

private:
	std::shared_ptr<Data> data;
	TextureType type;
};

struct Texture::Data
{
	Data() : id(0) {}
	Data(GLuint id) : id(id) {}

	Data(const Data &other) = delete;
	Data &operator=(const Data &other) = delete;

	Data(Data &&d) noexcept
	{
		*this = std::move(d);
	}
	Data &operator=(Data &&other) noexcept
	{
		id = other.id;
		other.id = 0;
		return *this;
	}

	~Data();

	// Texture Id
	GLuint id;
};

struct TextureLoader
{
	Texture::Data LoadFromFile(const std::string &imageLocation);
	Texture LoadFromFile(TextureType type, const std::string &imageLocation);

	Texture LoadCubemap(const std::string &directory, const std::vector<std::string> &filename);
	Texture LoadHDR(const std::string &filename);
};
