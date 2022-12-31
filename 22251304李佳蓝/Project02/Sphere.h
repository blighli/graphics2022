#pragma once

#include "glm/glm.hpp"
#include <cmath>
#include <vector>

using namespace std;

class Sphere
{
private:
	int _numVertices;
	int _numIndices;

	vector<int> _indices;
	vector<glm::vec3> _vertices;
	vector<glm::vec2> _texCoords;
	vector<glm::vec3> _normals;
	vector<glm::vec3> _tangents;

	void init(int prec);
	float toRadians(float degree);

public:
	Sphere();
	Sphere(int prec);

	int getNumVertices();
	int getNumIndices();
	
	vector<int> getIndices();
	vector<glm::vec3> getVertices();
	vector<glm::vec2> getTexCoords();
	vector<glm::vec3> getNormals();
	vector<glm::vec3> getTangents();

};

