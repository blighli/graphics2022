/*###################################################
##  文件: Mesh.h
##  文件说明： Mesh.cpp的头文件，用于导入fbx、obj等类型
的模型文件
#####################################################*/
#pragma once
#ifndef MYMESH_H
#define MYMESH_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;	// OpenGL纹理ID，绘制时绑定该纹理ID就可以使用该纹理进行绘制
	string type;		// 纹理的类型
	string path;		// 纹理图片文件的路径，我们储存纹理的路径用于与其它纹理进行比较
};

class Mesh {
public:
	vector<Vertex> vertices;			// 网格的顶点（指明网格有哪些点）
	vector<unsigned int> indices;		// 网格顶点的索引（指明网格哪些顶点连在一起组成一个个面）
	vector<Texture> textures;			// 该网格所用到的贴图，可以有多种、多个，如漫反射贴图、镜面反射贴图
	unsigned int VAO;					// 该网格的VAO（顶点数组对象）的ID，绘制网格时绑定该VAO，就可以调用glDrawArray或DrawElement进行绘制
	vector<glm::vec3> positions;
	// 为当前Mesh的vertices、indices、textures赋值，并创建和绑定VAO、VBO、EBO等（即：为VAO赋值）
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<glm::vec3> positions);
	

	// 给Fragment Shader的uniform sampler赋值
	void Draw(Shader shader);

private:
	unsigned int VBO, EBO, pVBO;	// pVBO：存储模型位置的VBO
	void setupMesh();
	
};
#endif