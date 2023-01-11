#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
	bool      operator==(const Vertex &other) const
	{
		return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
	}
};

namespace std
{
template <>
struct hash<Vertex>
{
	size_t operator()(Vertex const &vertex) const
	{
		return ((hash<glm::vec3>()(vertex.pos) ^
		         (hash<glm::vec3>()(vertex.normal) << 1)) >>
		        1) ^
		       (hash<glm::vec2>()(vertex.texCoord) << 1);
	}
};
}        // namespace std

class Mesh
{
  public:
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;

	void draw(Shader shader);
	void setupMesh();
	void free();
	void load(const std::string path);

  private:
	unsigned int mVAO, mVBO, mEBO;
};
