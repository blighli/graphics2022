#pragma once
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "Material.h"

constexpr int MAX_BONES_PER_VERTEX = 4;

struct Vertex
{
	Vertex() : position(0.0f), normal(0.0f), texCoords(0.0), tintColor(1.0f){};

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tintColor;
};

struct VertexBoneData
{
	VertexBoneData() : boneIds{0}, boneWeights{0} {}

	unsigned int boneIds[MAX_BONES_PER_VERTEX];
	float boneWeights[MAX_BONES_PER_VERTEX];

	void addWeight(unsigned id, float weight)
	{
		for (int i = 0; i != MAX_BONES_PER_VERTEX; i++)
		{
			if (boneWeights[i] == 0)
			{
				boneIds[i] = id;
				boneWeights[i] = weight;
				return;
			}
		}

		printf("WARNING: More than %d bones\n", MAX_BONES_PER_VERTEX);
	}
};

struct BoneData
{
	// Mesh space to Local bone space
	glm::mat4 boneOffset;
	unsigned int nodeId;
};

class Mesh
{
public:
	struct Data;
	struct BoundingSphere
	{
		BoundingSphere(glm::vec3 p, float r) : center(p), radius(r) {}
		BoundingSphere() : center(), radius() {}

		glm::vec3 center;
		float radius;
	};

	Mesh();
	Mesh(std::shared_ptr<Data> data, Material material);

	void GenVAO() const;

	// Remember To GenVAO
	void Draw(int instanced = 0) const;

	/*!
	 * @brief Gets the transforms of each bone in boneData given the position
	 * @param nodeTransforms  The transforms of all of the nodes in the model.
	 * @return Matrices for each bone which transform from the bone's bind-pose space to the bone's new space (given by nodeTransforms[bone.nodeId]).
	 * The values in the returned vector directly correspond to the bones in this mesh's boneData vector.
	 */
	std::vector<glm::mat4> GetBoneTransforms(const std::vector<glm::mat4> &nodeTransforms) const;

	unsigned int GetIndicesCount() const;

	GLuint GetVAO() const;

	Material material;

	BoundingSphere boundingSphere;
	bool hasBoundingSphere;

	std::string name;
	bool hasVertexBoneData;

private:
	std::shared_ptr<Data> data;
};

struct Mesh::Data
{
	Data() : VAO(0), VBO(0), VBO_bone(0), EBO(0), nVertices(0), nIndices(0), boneData(), boneTransforms() {}

	Data(const Data &other) = delete;
	Data &operator=(const Data &other) = delete;

	Data(Data &&d) noexcept
	{
		*this = std::move(d);
	}
	Data &operator=(Data &&other) noexcept
	{
		VAO = other.VAO;
		VBO = other.VBO;
		VBO_bone = other.VBO_bone;
		nVertices = other.nVertices;
		nIndices = other.nIndices;
		boneData = std::move(other.boneData);
		boneTransforms = std::move(other.boneTransforms);

		other.VAO = 0;
		other.VBO = 0;
		other.VBO_bone = 0;
		other.nVertices = 0;
		other.nIndices = 0;

		return *this;
	}

	~Data();

	/*! Vertex array object used to draw this model. */
	GLuint VAO;

	/* Vertex buffer object, containing Vertex structs. */
	GLuint VBO;

	/*! Vertex buffer object containing VertexBoneData structs. */
	GLuint VBO_bone;

	/*! Element buffer object. */
	GLuint EBO;

	/*! Total number of vertices in VBO. */
	GLuint nVertices;

	/*! Total number of indices in EBO. */
	GLuint nIndices;

	/*! Bone data of the mesh. */
	std::vector<BoneData> boneData;

	/*! Cached transforms returned from internal method. */
	std::vector<glm::mat4> boneTransforms;
};

class aiMesh;
class MeshBuilder
{
public:
	static Mesh BuildFromData(const std::shared_ptr<aiMesh> data, const Material &material, bool computeBoundingSphere = false);
	static Mesh BuildFromVertices(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, const Material &material, bool computeBoundingSphere = false);
	static Mesh BuildFromVertices(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, const Material &material, const std::vector<VertexBoneData> &vertexBoneData, const std::vector<BoneData> &boneData, bool computeBoundingSphere = false);
};