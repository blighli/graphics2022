#include "Mesh.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

void Mesh::draw(Shader shader)
{
	// 绘制网格
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);
}

void Mesh::free()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

void Mesh::load(const std::string path)
{
	//顶点去重。顶点：索引
	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	//attrib 容器记录所有的位置、法线和纹理坐标到attrib.vertices、attrib.normals 和attrib.texcoords 向量
	tinyobj::attrib_t attrib;
	//shapes 容器包含所有的对象及其面.每个面由一组顶点组成，每个顶点包含其位置、法线和纹理坐标属性的索引
	std::vector<tinyobj::shape_t> shapes;
	//
	std::vector<tinyobj::material_t> materials;
	std::string                      warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	for (const auto &shape : shapes)
	{
		//index 变量是tinyobj::index_t类型，它包含vertex_index、normal_index 和texcoord_index 成员
		for (const auto &index : shape.mesh.indices)
		{
			Vertex vertex = {};

			//不幸的是，attrib.vertices 数组是float值，不是glm::vec3，所以你需要将索引乘以3。
			//类似的，每个项有2个纹理坐标成员。
			//0、1和2偏移量用于存取XYZ成员，或UV成员（当是纹理坐标的时候）
			vertex.pos = {
			    attrib.vertices[3 * index.vertex_index + 0],
			    attrib.vertices[3 * index.vertex_index + 1],
			    attrib.vertices[3 * index.vertex_index + 2]};

			vertex.normal = {
			    attrib.normals[3 * index.normal_index + 0],
			    attrib.normals[3 * index.normal_index + 1],
			    attrib.normals[3 * index.normal_index + 2],
			};

			//OBJ格式假设坐标系统的竖直坐标的0表示image的底部，但是我们加载image到Vulkan的时候是按从顶到底的顺序的，这样0就指的是image的顶部。
			//通过翻转纹理坐标的竖直成员来解决这个问题
			vertex.texCoord = {
			    attrib.texcoords[2 * index.texcoord_index + 0],
			    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

			//是否已经出现过该vertex
			if (uniqueVertices.count(vertex) == 0)
			{
				//该vertex的索引为多少
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	setupMesh();
}
