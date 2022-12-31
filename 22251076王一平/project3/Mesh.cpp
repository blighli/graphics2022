/*###################################################
##  文件: Mesh.cpp
##  文件说明： 用于导入fbx、obj等类型的模型文件
#####################################################*/
#include "Mesh.h"

// 为当前Mesh的vertices、indices、textures赋值，并创建和绑定VAO、VBO、EBO等（即：为VAO赋值）
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<glm::vec3> positions)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->positions = positions;
	setupMesh();
}

// 给Fragment Shader的uniform sampler赋值
void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); 
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

		// 给Fragment Shader的uniform sampler赋值
		glUniform1i(glGetUniformLocation(shader.programID, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// GL_TRIANGLES为图元；indices.size()表示一个实例（instance）包含多少顶点
	// GL_UNSIGNED_INT表示数组索引的数据类型；0表示第一个索引的位置
	// positions.size()表示要绘制多少个实例
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, positions.size());
	
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}


void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// 设置不同实例（Instance）位置的VBO
	glGenBuffersARB(1, &pVBO);
	glBindBufferARB(GL_ARRAY_BUFFER, pVBO);
	glBufferDataARB(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointerARB(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArrayARB(3);
	// 指明layout(location = 3)的数据在绘制不同实例时才改变
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

