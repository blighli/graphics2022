// Mesh：使用Learn OpenGL网站提供的头文件

#ifndef MESH_H
#define MESH_H
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include <string>
#include <vector>
using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	Vertex(glm::vec3 pos, glm::vec3 nor);
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	// mesh Data
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;
	unsigned int VAO;

	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures=vector<Texture>());

	// render the mesh
	void Draw(Shader& shader);

private:
	// render data
	unsigned int VBO, EBO;

	// initializes all the buffer objects/arrays
	void setupMesh();
};
#endif