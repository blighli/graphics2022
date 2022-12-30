/*###################################################
##  文件: Model.h
##  文件说明： Model.cpp的头文件，用于导入fbx、obj等类型
的模型文件
#####################################################*/
#pragma once
#ifndef MYMODEL_H
#define MYMODEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

extern unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;				// 模型包含的所有Mesh
	string directory;
	bool gammaCorrection;				
	vector<glm::vec3> positions;		// 模型的位置

	Model(string const &path, vector<glm::vec3> positions, bool gamma = false);

	Model();
	// 绘制模型中所有的Mesh  所有Mesh使用的都是同一个Shader模板
	void Draw(Shader shader);

private:
	void loadModel(string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	
	// 将Assimp的数据解析到Mesh类中
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	
};



#endif