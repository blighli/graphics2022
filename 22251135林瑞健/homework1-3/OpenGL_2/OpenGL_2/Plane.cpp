#include<glad/glad.h>
#include <vector>
#include "Plane.h"
#include "Vertex.h"
#include "Object.h"

Plane::Data::Data(const Plane& plane)
{
	this->plane = &plane;
	this->vertexIndex = UNDEFINED;
	this->vertexTextureIndex = UNDEFINED;
	this->normalVectorIndex = UNDEFINED;
}

Plane::Data::Data(const Plane& plane, const GLint vertexIndex, const GLint vertexTextureIndex, const GLint normalVectorIndex)
{
	this->plane = &plane;
	this->vertexIndex = UNDEFINED;
	this->vertexTextureIndex = UNDEFINED;
	this->normalVectorIndex = UNDEFINED;
	setVertexIndex(vertexIndex);
	setVertexTextureIndex(vertexTextureIndex);
	setNormalVectorIndex(normalVectorIndex);
}

Plane::Data::~Data()
{
	
}

int Plane::Data::setVertexIndex(const GLint index)
{
	if (index <= plane->getObject()->getVertices().size())
	{
		this->vertexIndex = index;
		return OPERATION_SUSSESS;
	}
	else
	{
		return OPERATION_FAILURE;
	}
}

int Plane::Data::setVertexTextureIndex(const GLint index)
{
	return OPERATION_SUSSESS;
}

int Plane::Data::setNormalVectorIndex(const GLint index)
{
	if (index <= plane->getObject()->getVertexNormals().size())
	{
		this->normalVectorIndex = index;
		return OPERATION_SUSSESS;
	}
	else
	{
		return OPERATION_FAILURE;
	}
}

GLint Plane::Data::getVertexIndex() const
{
	return this->vertexIndex;
}

GLint Plane::Data::getVertexTextureIndex() const
{
	return this->vertexTextureIndex;
}

GLint Plane::Data::getNormalVectorIndex() const
{
	return this->normalVectorIndex;
}

const Vertex& Plane::getVertex(const GLint index) const
{
	return object->getVertices()[index - 1];
}

Plane::Plane()
{
}

Plane::~Plane()
{
}

const Plane::Data Plane::getData(const int index) const
{
	if (index < 3)
	{
		return datas[index];
	}
}

int Plane::addData(Data data)
{
	if (datas.size() <= 3)
	{
		datas.push_back(data);
		return OPERATION_SUSSESS;
	}
	else
	{
		return OPERATION_FAILURE;
	}
}

void Plane::clearData()
{
	datas.clear();
}

Plane& Plane::operator=(const Plane& plane)
{
	if (this == &plane)
		return *this;
	this->datas.clear();
	for (int i = 0; i < plane.datas.size(); i++)
	{
		this->datas.push_back(plane.datas[i]);
	}
	return *this;
}

bool Plane::operator==(const Plane& plane) const
{
	bool result = true;
	if (this->datas.size() != plane.datas.size())
		return false;
	for (int i = 0; i < this->datas.size(); i++)
	{
		result = result && (this->datas[i].getVertexIndex() == plane.datas[i].getVertexIndex());
		result = result && (this->datas[i].getVertexTextureIndex() == plane.datas[i].getVertexTextureIndex());
		result = result && (this->datas[i].getNormalVectorIndex() == plane.datas[i].getNormalVectorIndex());
		if (!result)
			return result;
	}
	return result;
}

bool Plane::operator!=(const Plane& plane) const
{
	return !operator==(plane);
}

void Plane::setObject(const Object& object)
{
	this->object = &object;
}

const Object* Plane::getObject() const
{
	return object;
}