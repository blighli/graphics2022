#pragma once
#include <vector>
#include<cmath>
#include <glad\glad.h>
#ifndef SPHERE_H
#define SPHERE_H



class sphere
{
public:
	int Y_SEGMENTS = 20;
	int X_SEGMENTS = 20;
	glm::vec3 core = glm::vec3(0.0, 0.0, 0.0);
	float radius = 1.0f;
	sphere();
	sphere(int x_s, int y_s, glm::vec3 c, float r):Y_SEGMENTS(y_s),X_SEGMENTS(x_s),
	core(c),radius(r)
	{}
	sphere(int x_s, int y_s);
	~sphere();


	std::vector<float> getVertices();
	std::vector<int> getIndices();
	std::vector<float> getNormals();
	std::vector<float> getTexCoords();

private:

};

sphere::sphere()
{
}
sphere::sphere(int x_s, int y_s)
{
	X_SEGMENTS = x_s;
	Y_SEGMENTS = y_s;
}
sphere::~sphere()
{
}

std::vector<float> sphere::getVertices()
{
	const GLfloat PI = 3.14159265358979323846f;
	std::vector<float> sphereVertices;
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = radius* std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI)+core.x;
			float yPos = radius*std::cos(ySegment * PI)+core.y;
			float zPos = radius*std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI)+core.z;
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
		}
	}
	return sphereVertices;
}
std::vector<int> sphere::getIndices()
{
	std::vector<int> sphereIndices;
	
	for (int j = 0; j < Y_SEGMENTS; j++)
	{
		for (int i = 0; i < X_SEGMENTS; i++)
		{
			//sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);//左上
			//sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);//左下
			//sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);//右下
			//sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);//左上
			//sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);//右下
			//sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);//右上


			sphereIndices.push_back((j + 1) * (X_SEGMENTS + 1) + i);
			sphereIndices.push_back((j + 1) * (X_SEGMENTS + 1) + i + 1);
			sphereIndices.push_back(j * (X_SEGMENTS + 1) + i + 1);

			sphereIndices.push_back(j * (X_SEGMENTS + 1) + i + 1);
			sphereIndices.push_back(j * (X_SEGMENTS + 1) + i);
			sphereIndices.push_back((j + 1) * (X_SEGMENTS + 1) + i);
		}
	}
	return sphereIndices;
}
std::vector<float> sphere::getNormals()
{
	const GLfloat PI = 3.14159265358979323846f;
	std::vector<float> sphereNormals;
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			glm::vec3 temp=glm::normalize(glm::vec3(xPos, yPos, zPos));
			sphereNormals.push_back(temp.x);
			sphereNormals.push_back(temp.y);
			sphereNormals.push_back(temp.z);
		}
	}
	return sphereNormals;
}
std::vector<float> sphere::getTexCoords()
{
	const GLfloat PI = 3.14159265358979323846f;
	std::vector<float> sphereTexCoords;
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			sphereTexCoords.push_back(1-xSegment);
			sphereTexCoords.push_back(ySegment);
		}
	}
	return sphereTexCoords;
}
#endif