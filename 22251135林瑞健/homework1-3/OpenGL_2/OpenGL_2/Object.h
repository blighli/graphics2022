#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Vertex.h"
#include "Plane.h"
#include "Shader.h"
#include "Texture.h"

class Plane;

// 物体
class Object
{
public:
	Object();
	~Object();
	// 从obj文件中加载Object
	int loadObject(std::istream& input);
	// 执行绘制前的准备工作
	void draw();
	// 添加点
	int addVertex(const Vertex& vertex);
	// 添加平面
	int addPlane(const Plane& plane);
	// 删除点
	int deleteVertex(const Vertex& vertex);
	// 删除平面
	int deletePlane(const Plane& plane);
	// 删除点
	int deleteVertex(const int position);
	// 删除平面
	int deletePlane(const int position);
	// 添加点法向量
	int addVertexNormal(const glm::vec3& normal);
	// 删除点法向量
	int deleteVertexNormal(const int position);
	// 清空点法向量
	void clearVertexNormal();
	// 添加着色器
	void addShader(Shader& shader);
	// 清除着色器
	void clearShaders();
	// 清除点
	void clearVertices();
	// 清除平面
	void clearPlanes();
	// 添加贴图
	void addTexture(Texture& texture);
	// 清空贴图
	void clearTextures();
	// 获取当前状态下的变换矩阵
	const glm::mat4& getTransformation() const;
	// 重置变换矩阵
	void resetTranslation();
	// 获取ShaderProgram
	GLuint getShaderProgram() const;
	// 获取所有贴图
	const std::vector<Texture>& getTextures();
	// 获取VAO
	GLuint getVAO() const;
	// 获取物体原点在世界坐标系中的坐标
	glm::vec3 getPosition();
	// 获取物体相对世界坐标系的坐标轴转动的角度
	glm::vec3 getSelfRotation();
	// 平移（相对于物体坐标系）
	void translate(const glm::vec3& direction);
	// 旋转（相对于物体坐标系）
	void selfRotate(const float degree, const glm::vec3& axis);
	// 旋转（相对于世界坐标系），暂不可用
	void rotate(const float degree, const glm::vec3& axis);
	// 获取所有点
	const std::vector<Vertex>& getVertices() const;
	// 获取所有平面
	const std::vector<Plane>& getPlanes() const;
	// 获取所有点法向量
	const std::vector<glm::vec3>& getVertexNormals() const;
	const static int OPERATION_SUSSESS = 1;
	const static int OPERATION_FAILURE = 0;
	const static int UNDEFINED = UINT_MAX;

private:
	// 顶点
	std::vector<Vertex> vertices;
	// 平面
	std::vector<Plane> planes;
	// 着色器
	std::vector<Shader> shaders;
	// 贴图
	std::vector<Texture> textures;
	// 点法向量
	std::vector<glm::vec3> vertexNormals;
	// 变换矩阵
	glm::mat4 transformation;
	// 着色器程序
	GLuint shaderProgram;
	// Vertex Array Object
	GLuint vao;
	// 模型的当前位置
	glm::vec3 position;
	// 模型的旋转向量（绕x，y，z轴）
	glm::vec3 rotation;
	// 初始化着色器
	void initShaders();
	// 初始化着色器程序
	void initShaderProgram();
	// 初始化VAO
	void initVAO();
	// 初始化贴图
	void initTextures();
	// 生成顶点数据
	void prepareVertices(std::vector<GLfloat>& verticesInfo);
	// 生成顶点索引数据
	void prepareVertexIndices(std::vector<GLint>& vertexIndices);
	// 生成顶点法向量数据
	void prepareVertexNormals(std::vector<GLfloat>& vertexNormalInfo);
	// 生成顶点法向量索引数据
	void prepareVertexNormalIndices(std::vector<GLint>& vertexNormalIndices);
};
#endif // !OBJECT_H