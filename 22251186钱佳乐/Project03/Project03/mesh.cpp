#include "Mesh.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

void Mesh::draw(Shader shader)
{
	// ��������
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
	//����ȥ�ء����㣺����
	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	//attrib ������¼���е�λ�á����ߺ��������굽attrib.vertices��attrib.normals ��attrib.texcoords ����
	tinyobj::attrib_t attrib;
	//shapes �����������еĶ�������.ÿ������һ�鶥����ɣ�ÿ�����������λ�á����ߺ������������Ե�����
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
		//index ������tinyobj::index_t���ͣ�������vertex_index��normal_index ��texcoord_index ��Ա
		for (const auto &index : shape.mesh.indices)
		{
			Vertex vertex = {};

			//���ҵ��ǣ�attrib.vertices ������floatֵ������glm::vec3����������Ҫ����������3��
			//���Ƶģ�ÿ������2�����������Ա��
			//0��1��2ƫ�������ڴ�ȡXYZ��Ա����UV��Ա���������������ʱ��
			vertex.pos = {
			    attrib.vertices[3 * index.vertex_index + 0],
			    attrib.vertices[3 * index.vertex_index + 1],
			    attrib.vertices[3 * index.vertex_index + 2]};

			vertex.normal = {
			    attrib.normals[3 * index.normal_index + 0],
			    attrib.normals[3 * index.normal_index + 1],
			    attrib.normals[3 * index.normal_index + 2],
			};

			//OBJ��ʽ��������ϵͳ����ֱ�����0��ʾimage�ĵײ����������Ǽ���image��Vulkan��ʱ���ǰ��Ӷ����׵�˳��ģ�����0��ָ����image�Ķ�����
			//ͨ����ת�����������ֱ��Ա������������
			vertex.texCoord = {
			    attrib.texcoords[2 * index.texcoord_index + 0],
			    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

			//�Ƿ��Ѿ����ֹ���vertex
			if (uniqueVertices.count(vertex) == 0)
			{
				//��vertex������Ϊ����
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	setupMesh();
}
