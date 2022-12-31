#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include "Vertex.h"


Vertex::Vertex()
{
	x = UNDEFINED;
	y = UNDEFINED;
	z = UNDEFINED;
}

Vertex::Vertex(const GLfloat x, const GLfloat y, const GLfloat z)
{
	setX(x);
	setY(y);
	setZ(z);
}

Vertex::~Vertex()
{
}

int Vertex::setX(const GLfloat x)
{
	if (abs(x) <= 1)
	{
		this->x = x;
		return OPERATION_SUSSESS;
	}
	return OPERATION_FAILURE;
}

int Vertex::setY(const GLfloat y)
{
	if (abs(y) <= 1)
	{
		this->y = y;
		return OPERATION_SUSSESS;
	}
	return OPERATION_FAILURE;
}

int Vertex::setZ(const GLfloat z)
{
	if (abs(z) <= 1)
	{
		this->z = z;
		return OPERATION_SUSSESS;
	}
	return OPERATION_FAILURE;
}

GLfloat Vertex::getX() const
{
	return this->x;
}

GLfloat Vertex::getY() const
{
	return this->y;
}

GLfloat Vertex::getZ() const
{
	return this->z;
}

Vertex& Vertex::operator=(const Vertex& vertex)
{
	if (this == &vertex)
		return *this;
	this->x = vertex.x;
	this->y = vertex.y;
	this->z = vertex.z;
	return *this;
}

bool Vertex::operator==(const Vertex& vertex) const
{
	return (this->x == vertex.x) && (this->y == vertex.y) && (this->z == vertex.z);
}

bool Vertex::operator!=(const Vertex& vertex) const
{
	return !operator==(vertex);
}

Vertex::operator glm::vec3() const
{
	return glm::vec3(x, y, z);
}

Vertex::operator glm::vec4() const
{
	return glm::vec4(x, y, z, 1);
}