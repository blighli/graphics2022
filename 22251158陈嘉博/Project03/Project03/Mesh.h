#pragma once
#include <glad/glad.h> // ����ͷ�ļ�
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
//����
struct Vertex {
	// λ��
	glm::vec3 Position;
	// ������
	glm::vec3 Normal;
	// ��������
	glm::vec2 TexCoords;
	// u����
	glm::vec3 Tangent;
	// v����
	glm::vec3 Bitangent;
};
//����
struct Texture {
	unsigned int id;
	string type;
	string path;
};
//Mesh��
class Mesh {
public:
	/*  Mesh ����  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	/*  ����  */
	// ���캯�� ���������� ���� ����
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		// ��������ӵ�������б�������ݣ����ö��㻺������������ָ�롣
		setupMesh();
	}

	// ������ģ��
	void Draw(Shader shader)
	{
		// ���ʵ�������
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // ��ǰ�����ʵ�������Ԫ
											  // ��ȡ�����ţ�diffuse_textureN�е�N��
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);
			// ���ڽ�����������Ϊ��ȷ������Ԫ
			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			// ��������
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		// ������
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// һ��������Ͻ�һ�����û�Ĭ��ֵ���Ǻܺõ���������
		glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  ��Ⱦ����  */
	unsigned int VBO, EBO;

	/*  ����    */
	// ��ʼ�����л���������/����
	void setupMesh()
	{
		// ����������/����
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		//�����ݼ��ص����㻺������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// ���ڽṹ��һ���ô������ǵ��ڴ沼�ֶ��������������˳��ġ�
		// ��������ǿ��Լ򵥵ؽ�ָ�봫�ݸ��ṹ��������������ת��Ϊglm :: vec3 / 2���飬�������ٴ�ת��Ϊ3/2��������ת��Ϊ�ֽ����顣
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		// ���ö�������ָ��
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// ������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// u����
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// v����
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glBindVertexArray(0);
	}
};
