#pragma once
#include <glad/glad.h>                 //����ͷ�ļ� 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION        //ԭ����ûд
#include "stb_image.h"
#include <assimp/Importer.hpp>        //assimp��ͷ�ļ�
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
//���ļ��ж�ȡ����
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
//Model��
class Model
{
public:
	/*  Model���� */
	//�洢��ĿǰΪֹ���ص����������Ż���ȷ�������ᱻ���ض�Ρ�
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	/*  ����  */
	// ���캺��,��Ҫһ��3Dģ�͵��ļ�·��
	Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}

	// ����ģ�ͣ��Ӷ�������������
	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

private:
	/*  ����   */
	// ���ļ�����֧��ASSIMP��չ��ģ�ͣ��������ɵ�����洢������ʸ���С�
	void loadModel(string const &path)
	{
		// ͨ��ASSIMP���ļ�
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// ������
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // �������0
		{
			cout << "����::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// �����ļ�·����Ŀ¼·��
		directory = path.substr(0, path.find_last_of('/'));

		// �Եݹ鷽ʽ����ASSIMP�ĸ��ڵ�
		processNode(scene->mRootNode, scene);
	}

	// �Եݹ鷽ʽ����ڵ㡣 ����λ�ڽڵ㴦��ÿ���������񣬲������ӽڵ㣨����У����ظ��˹��̡�
	void processNode(aiNode *node, const aiScene *scene)
	{
		// ����λ�ڵ�ǰ�ڵ��ÿ������
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// �ڵ��������������������������е�ʵ�ʶ���
			// ���������������ݣ��ڵ�ֻ��Ϊ������֯�ı��涫������ڵ�֮��Ĺ�ϵ����
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// �����Ǵ�����������������еĻ��������ǻ�ݹ鴦��ÿ���ӽڵ�
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// Ҫ��д������
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		// ����ÿ������Ķ���
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// ��������һ��ռλ����������Ϊassimpʹ�����Լ��������࣬����ֱ��ת��Ϊglm��vec3�࣬�����������Ƚ����ݴ��ݸ����ռλ��glm :: vec3��
			glm::vec3 vector;
			// λ��
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// ����
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// ��������
			if (mesh->mTextureCoords[0]) // �����Ƿ�����������ꣿ
			{
				glm::vec2 vec;
				// �������ɰ���8����ͬ���������ꡣ ��ˣ����Ǽ������ǲ���ʹ�ö�����Ծ��ж�����������ģ�ͣ�����������ǲ��õ�һ�����ϣ�0����
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// u����
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// v����
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		//���ڱ���ÿ�������棨һ������һ�������ε����񣩲�������Ӧ�Ķ���������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// ����������������������Ǵ洢������������
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// �ӹ�����
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// ���Ǽ�����ɫ���еĲ���������Լ���� ÿ������������Ӧ����Ϊ'texture_diffuseN'������N�Ǵ�1��MAX_SAMPLER_NUMBER�����кš�
		//ͬ�����������������������ܽ᣺
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. ��������ͼ
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. �߹���ͼ
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3.������ͼ
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. �߶���ͼ
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// ���ش���ȡ���������ݴ������������
		return Mesh(vertices, indices, textures);
	}

	// ���������͵����в������������δ�����������������
	// ������Ϣ��ΪTexture�ṹ���ء�
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// ���֮ǰ�Ƿ��������������ǣ��������һ�ε�������������������
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				//cout << textures_loaded[j].path <<endl;
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;// �Ѽ��ؾ�����ͬ�ļ�·��������������һ�����Ż�����
				}
			}
			if (!skip)
			{   // �����δ���������������
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  //����洢Ϊ����ģ�ͼ��ص�������ȷ�����ǲ�������ظ�����
			}
		}
		return textures;
	}
};
//���ļ���ȡ������
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;
	cout << path<< endl;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "�����޷��Ӵ�·������: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

