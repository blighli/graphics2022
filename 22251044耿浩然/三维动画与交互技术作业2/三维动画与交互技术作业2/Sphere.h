
#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include<cmath>

#define PI 3.1415926

class Sphere 
{

public:
	GLfloat verts[194400];//顶点*3    UV*2    法线*3//共8个分量
	GLuint indices[24300];
	GLfloat vertNum;

public:
	Sphere();
	~Sphere() {}

private:
	void pushVerts(int &index, float a, float b, float c);
	void pushVerts(int &index, float a, float b);
	void genCoor(float& x, float& y, float& z, float Zdegree, float XYdegree);
	void genUV(float& U, float& V, float Zdegree, float XYdegree);
};