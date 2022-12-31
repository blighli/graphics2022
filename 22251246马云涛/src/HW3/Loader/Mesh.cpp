#include <exception>
#include <glad/glad.h>
#include <glm/gtx/norm.hpp>
#include <assimp/mesh.h>

#include "Mesh.h"
#include "Material.h"
#include "RenderUtil.h"

Mesh::Data::~Data()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	if (VBO_bone)
	{
		glDeleteBuffers(1, &VBO_bone);
	}

	glDeleteVertexArrays(1, &VAO);
}

Mesh::Mesh() : data(), material(), hasVertexBoneData(false), hasBoundingSphere(false) {}

Mesh::Mesh(std::shared_ptr<Data> data, Material material)
	: data(data), material(material), hasVertexBoneData(data->boneData.size() > 0), hasBoundingSphere(false)
{
}

void Mesh::GenVAO() const
{
	glGenVertexArrays(1, &data->VAO);

	glBindVertexArray(data->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->EBO);
	// 顶点位置
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	// 顶点法线
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
	// 顶点纹理坐标
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

	if (this->hasVertexBoneData)
	{
		glBindBuffer(GL_ARRAY_BUFFER, data->VBO_bone);
		glEnableVertexAttribArray(7);
		glVertexAttribIPointer(7, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid *)0);
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid *)offsetof(VertexBoneData, boneWeights));
	}

	glBindVertexArray(0);

	glCheckError();
}

void Mesh::Draw(int instanced) const
{
	/*!
	 * @brief Don't forget to Gen VAO
	 */
	assert(data->VAO != 0);

	glBindVertexArray(data->VAO);

	if (instanced)
		glDrawElementsInstanced(GL_TRIANGLES, data->nIndices, GL_UNSIGNED_INT, 0, instanced);
	else
		glDrawElements(GL_TRIANGLES, data->nIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::vector<glm::mat4> Mesh::GetBoneTransforms(const std::vector<glm::mat4> &nodeTransforms) const
{
	if (nodeTransforms.size() <= 0)
	{
		return data->boneTransforms;
	}

	// Assume 0 is the root node
	glm::mat4 globalInverse = glm::inverse(nodeTransforms[0]);
	for (unsigned int i = 0; i < data->boneData.size(); i++)
	{
		const BoneData &boneData = data->boneData[i];
		glm::mat4 nodeTransform = nodeTransforms[boneData.nodeId];
		glm::mat4 boneOffset = boneData.boneOffset;
		glm::mat4 boneTransform = globalInverse * nodeTransform * boneOffset;
		data->boneTransforms[i] = boneTransform;
	}

	return data->boneTransforms;
}

unsigned int Mesh::GetIndicesCount() const
{
	return data->nIndices;
}

GLuint Mesh::GetVAO() const
{
	return data->VAO;
}

Mesh MeshBuilder::BuildFromData(const std::shared_ptr<aiMesh> data, const Material &material, bool computeBoundingSphere)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	for (unsigned i = 0; i < data->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(data->mVertices[i].x, data->mVertices[i].y, data->mVertices[i].z);
		vertex.normal = glm::vec3(data->mNormals[i].x, data->mNormals[i].y, data->mNormals[i].z);
		if (data->mTextureCoords[0])
		{
			vertex.texCoords = glm::vec2(data->mTextureCoords[0][i].x, data->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned i = 0; i < data->mNumFaces; i++)
	{
		aiFace face = data->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return BuildFromVertices(vertices, indices, material, computeBoundingSphere);
}

Mesh MeshBuilder::BuildFromVertices(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, const Material &material, bool computeBoundingSphere)
{
	return BuildFromVertices(vertices, indices, material, std::vector<VertexBoneData>(), std::vector<BoneData>(), computeBoundingSphere);
}

Mesh MeshBuilder::BuildFromVertices(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, const Material &material, const std::vector<VertexBoneData> &vertexBoneData, const std::vector<BoneData> &boneData, bool computeBoundingSphere)
{
	auto data = std::make_shared<Mesh::Data>();

	glGenBuffers(1, &data->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &data->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	if (vertexBoneData.size() > 0)
	{
		glGenBuffers(1, &data->VBO_bone);
		glBindBuffer(GL_ARRAY_BUFFER, data->VBO_bone);
		glBufferData(GL_ARRAY_BUFFER, vertexBoneData.size() * sizeof(VertexBoneData), &vertexBoneData[0], GL_STATIC_DRAW);
	}

	glCheckError();

	data->nVertices = vertices.size();
	data->nVertices = vertices.size();
	data->nIndices = indices.size();
	data->boneData = boneData;
	data->boneTransforms.resize(data->boneData.size());

	Mesh r = Mesh(data, material);

	if (computeBoundingSphere)
	{
		// Ritter bounding sphere
		glm::vec3 vmin, vmax;
		vmin = vmax = vertices[0].position;

		for (int i = 0; i != vertices.size(); i++)
		{
			if (vertices[i].position.x < vmin.x)
				vmin.x = vertices[i].position.x;
			if (vertices[i].position.y < vmin.y)
				vmin.y = vertices[i].position.y;
			if (vertices[i].position.z < vmin.z)
				vmin.z = vertices[i].position.z;

			if (vertices[i].position.x > vmax.x)
				vmax.x = vertices[i].position.x;
			if (vertices[i].position.y > vmax.y)
				vmax.y = vertices[i].position.y;
			if (vertices[i].position.z > vmax.z)
				vmax.z = vertices[i].position.z;
		}

		float xdiff = vmax.x - vmin.x;
		float ydiff = vmax.y - vmin.y;
		float zdiff = vmax.z - vmin.z;

		float diameter = glm::max(xdiff, glm::max(ydiff, zdiff));
		glm::vec3 center = (vmax + vmin) * 0.5f;
		float radius = diameter / 2;
		float sq_radius = radius * radius;

		for (int i = 0; i < vertices.size(); i++)
		{
			glm::vec3 point = vertices[i].position;

			glm::vec3 direction = point - center;
			float sq_distance = glm::length2(direction);

			if (sq_distance > sq_radius)
			{
				float distance = sqrt(sq_distance);

				float newRadius = (distance + radius) / 2;
				float k = (newRadius - radius) / distance;

				radius = newRadius;
				sq_radius = radius * radius;

				center -= k * direction;
			}
		}

		r.boundingSphere = Mesh::BoundingSphere(center, radius);
		r.hasBoundingSphere = true;
	}

	r.material.SetProperty("hasBone", vertexBoneData.size() == 0 ? 0 : 1);

	return std::move(r);
}