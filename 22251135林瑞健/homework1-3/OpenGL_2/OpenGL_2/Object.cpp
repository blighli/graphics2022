#include "Object.h"
#include "Plane.h"
#include "Vertex.h"
#include "StringUtil.h"
#include "Texture.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <thread>
#include <functional>
#include <mutex>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const double PI = 3.14159265358979323846;
static double calculateDegree(double _x, double _y);

Object::Object()
{
	shaderProgram = UNDEFINED;
	vao = UNDEFINED;
	resetTranslation();
}

Object::~Object()
{
	vertices.clear();
	planes.clear();
	shaders.clear();
	textures.clear();
	
	std::vector<Vertex>().swap(vertices);
	std::vector<Plane>().swap(planes);
	std::vector<Shader>().swap(shaders);
	std::vector<Texture>().swap(textures);
}

void Object::draw()
{
	initShaders();
	initShaderProgram();
	initVAO();
	initTextures();
}

int Object::loadObject(std::istream& input)
{
	if (input.bad())
	{
		std::cout << "Can't read the input" << '\n';
		return OPERATION_FAILURE;
	}
	if (input.good())
	{
		std::stringstream ss;
		ss << input.rdbuf();
		vertices.clear();
		planes.clear();
		vertexNormals.clear();
		while (!ss.eof())
		{
			std::string content;
			std::getline(ss, content);
			std::vector<std::string> data = StringUtil::split(content, "\\s+");
			if (data[0] == "#")
				continue;
			if (data[0] == "v")
			{
				Vertex* vertex = new Vertex(atof(data[1].c_str()), atof(data[2].c_str()), atof(data[3].c_str()));
				addVertex(*vertex);
				delete vertex;
			}
			if (data[0] == "vn")
			{
				glm::vec3 vertexNormal(atof(data[1].c_str()), atof(data[2].c_str()), atof(data[3].c_str()));
				addVertexNormal(vertexNormal);
			}
			if (data[0] ==  "f")
			{
				Plane* plane = new Plane();
				plane->setObject(*this);
				for (int i = 1; i < data.size(); i++)
				{
					std::vector<std::string> indexData = StringUtil::split(data[i], "/");
					std::vector<GLint> _indexData = { Plane::Data::UNDEFINED , Plane::Data::UNDEFINED , Plane::Data::UNDEFINED };
					for (int j = 0; j < 3; j++)
					{
						if (j < indexData.size())
						{
							if (StringUtil::isNumber(indexData[j]))
							{
								_indexData[j] = atoi(indexData[j].c_str());
							}
						}
					}
					Plane::Data planeData(*plane, _indexData[0], _indexData[1], _indexData[2]);
					plane->addData(planeData);
				}
				addPlane(*plane);
				delete plane;
			}
		}
	}
	
	return OPERATION_SUSSESS;
}

int Object::addVertex(const Vertex& vertex)
{
	vertices.push_back(vertex);
	return OPERATION_SUSSESS;
}

int Object::addPlane(const Plane& plane)
{
	planes.push_back(plane);
	return OPERATION_SUSSESS;
}

int Object::deleteVertex(const Vertex& vertex)
{
	std::vector<Vertex>::iterator position = std::find(vertices.begin(), vertices.end(), vertex);
	if (position == vertices.end())
		return OPERATION_FAILURE;
	else
	{
		vertices.erase(position);
		return OPERATION_SUSSESS;
	}
}

int Object::deletePlane(const Plane& plane)
{
	std::vector<Plane>::iterator position = std::find(planes.begin(), planes.end(), plane);
	if (position == planes.end())
		return OPERATION_FAILURE;
	else
	{
		planes.erase(position);
		return OPERATION_SUSSESS;
	}
}

int Object::deleteVertex(const int position)
{
	if (position >= vertices.size())
		return OPERATION_FAILURE;
	else
	{
		vertices.erase(vertices.begin() + position);
		return OPERATION_SUSSESS;
	}
}

int Object::deletePlane(const int position)
{
	if (position > planes.size())
		return OPERATION_FAILURE;
	else
	{
		planes.erase(planes.begin() + position);
		return OPERATION_SUSSESS;
	}
}

int Object::addVertexNormal(const glm::vec3& normal)
{
	vertexNormals.push_back(normal);
	return OPERATION_SUSSESS;
}

int Object::deleteVertexNormal(const int position)
{
	if (position > vertexNormals.size())
		return OPERATION_FAILURE;
	else
	{
		vertexNormals.erase(vertexNormals.begin() + position);
		return OPERATION_SUSSESS;
	}
}

void Object::clearVertexNormal()
{
	vertexNormals.clear();
}

void Object::addShader(Shader& shader)
{
	shaders.push_back(shader);
	try
	{
		delete &shader;
	}
	catch (const std::exception&)
	{

	}
}

void Object::clearShaders()
{
	shaders.clear();
}

void Object::clearVertices()
{
	vertices.clear();
}

void Object::clearPlanes()
{
	planes.clear();
}

void Object::addTexture(Texture& texture)
{
	textures.push_back(texture);
	try
	{
		delete &texture;
	}
	catch (const std::exception&)
	{

	}
}

void Object::clearTextures()
{
	textures.clear();
}

const std::vector<Vertex>& Object::getVertices() const
{
	return vertices;
}

const std::vector<Plane>& Object::getPlanes() const
{
	return planes;
}

const std::vector<glm::vec3>& Object::getVertexNormals() const
{
	return vertexNormals;
}

GLuint Object::getShaderProgram() const
{
	return shaderProgram;
}

const std::vector<Texture>& Object::getTextures()
{
	return textures;
}

GLuint Object::getVAO() const
{
	return vao;
}

const glm::mat4& Object::getTransformation() const
{
	return transformation;
}

glm::vec3 Object::getPosition()
{
	glm::vec4 center(0, 0, 0, 1);
	center = transformation * center;
	position = glm::vec3(center.x, center.y, center.z);
	return position;
}

glm::vec3 Object::getSelfRotation()
{
	glm::vec4 xAxis(1, 0, 0, 0);
	glm::vec4 yAxis(0, 1, 0, 0);
	glm::vec4 zAxis(0, 0, 1, 0);
	
	glm::vec4 current = transformation * xAxis;
	double zDegree = calculateDegree(current.x, current.y);
	current = transformation * zAxis;
	double yDegree = calculateDegree(current.z, current.x);
	current = transformation * yAxis;
	double xDegree = calculateDegree(current.y, current.z);
	rotation = glm::vec3(xDegree, yDegree, zDegree);
	return rotation;
}

void Object::translate(const glm::vec3& direction)
{
	glm::mat4 t(1.0F);
	transformation *= glm::translate(t, direction);
}

void Object::selfRotate(const float degree, const glm::vec3& axis)
{
	glm::mat4 _rotate(1.0F);
	_rotate = glm::rotate(_rotate, glm::radians(degree), axis);
	transformation *= _rotate;
}

void Object::rotate(const float degree, const glm::vec3& axis)
{
	getSelfRotation();
	glm::mat4 rotateToDefault(1.0F);
	glm::mat4 _rotate(1.0F);
	glm::mat4 rotateBack(1.0F);
	rotateToDefault = glm::rotate(rotateToDefault, glm::radians(-rotation.x), glm::vec3(1, 0, 0));
	rotateToDefault = glm::rotate(rotateToDefault, glm::radians(-rotation.y), glm::vec3(0, 1, 0));
	rotateToDefault = glm::rotate(rotateToDefault, glm::radians(-rotation.z), glm::vec3(0, 0, 1));
	_rotate = glm::rotate(_rotate, glm::radians(degree), axis);
	rotateBack = glm::rotate(rotateBack, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	rotateBack = glm::rotate(rotateBack, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	rotateBack = glm::rotate(rotateBack, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	transformation *= rotateBack * _rotate * rotateToDefault;
}

void Object::resetTranslation()
{
	transformation = glm::mat4(1.0F);
	position = glm::vec3(0.0, 0.0, 0.0);
	rotation = glm::vec3(0.0, 0.0, 0.0);
}

void Object::initShaders()
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (!shaders[i].compile())
		{
			std::cerr << "(Shader: " << i << ", id: " << shaders[i].getId() << ")\n";
		}
	}
}

void Object::initShaderProgram()
{
	shaderProgram = glCreateProgram();
	for (int i = 0; i < shaders.size(); i++)
	{
		glAttachShader(shaderProgram, shaders[i].getId());
	}
	glLinkProgram(shaderProgram);
	GLint success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: Prepare Shader Program Failed\n" << infoLog << '\n';
	}
	glUseProgram(shaderProgram);
	for (int i = 0; i < shaders.size(); i++)
	{
		glDeleteShader(shaders[i].getId());
	}
}

void Object::initVAO()
{
	GLuint vbo, ebo, vnbo, enbo;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vnbo);
	glGenBuffers(1, &enbo);
	glBindVertexArray(vao);
	std::vector<GLfloat> verticesInfo;
	std::vector<GLint> vertexIndices;
	std::vector<GLint> vertexNormalIndices;
	std::vector<GLfloat> vertexNormalInfo;
	std::thread t1(std::mem_fn(&Object::prepareVertices), this, std::ref(verticesInfo));
	std::thread t2(std::mem_fn(&Object::prepareVertexIndices), this, std::ref(vertexIndices));
	std::thread t3(std::mem_fn(&Object::prepareVertexNormals), this, std::ref(vertexNormalInfo));
	std::thread t4(std::mem_fn(&Object::prepareVertexNormalIndices), this, std::ref(vertexNormalIndices));
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	if (!vertices.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesInfo[0]) * verticesInfo.size(), &(verticesInfo[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	}
	
	if (!planes.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices[0]) * vertexIndices.size(), &(vertexIndices[0]), GL_STATIC_DRAW);
	}
	if (!vertexNormals.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, vnbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormalInfo[0]) * vertexNormalInfo.size(), &(vertexNormalInfo[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
	}
	if (!planes.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexNormalIndices[0]) * vertexNormalIndices.size(), &(vertexNormalIndices[0]), GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::initTextures()
{
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].loadSource();
	}
}

void Object::prepareVertices(std::vector<GLfloat>& verticesInfo)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		verticesInfo.push_back(vertices[i].getX());
		verticesInfo.push_back(vertices[i].getY());
		verticesInfo.push_back(vertices[i].getZ());
	}
}

void Object::prepareVertexIndices(std::vector<GLint>& vertexIndices)
{
	for (int i = 0; i < planes.size(); i++)
	{
		vertexIndices.push_back(planes[i].getData(0).getVertexIndex() - 1);
		vertexIndices.push_back(planes[i].getData(1).getVertexIndex() - 1);
		vertexIndices.push_back(planes[i].getData(2).getVertexIndex() - 1);
	}
}

void Object::prepareVertexNormals(std::vector<GLfloat>& vertexNormalInfo)
{
	for (int i = 0; i < vertexNormals.size(); i++)
	{
		vertexNormalInfo.push_back(vertexNormals[i].x);
		vertexNormalInfo.push_back(vertexNormals[i].y);
		vertexNormalInfo.push_back(vertexNormals[i].z);
	}
}

void Object::prepareVertexNormalIndices(std::vector<GLint>& vertexNormalIndices)
{
	for (int i = 0; i < planes.size(); i++)
	{
		vertexNormalIndices.push_back(planes[i].getData(0).getNormalVectorIndex() - 1);
		vertexNormalIndices.push_back(planes[i].getData(1).getNormalVectorIndex() - 1);
		vertexNormalIndices.push_back(planes[i].getData(2).getNormalVectorIndex() - 1);
	}
}

static double calculateDegree(double _x, double _y)
{
	double result = 0.0;
	if (std::abs(_x) < 1e-5)
		_x = 0;
	if (std::abs(_y) < 1e-5)
		_y = 0;
	result = std::atan2(_y, _x) / PI * 180;
	if (result < 0)
		result += 360;
	return result;
}