#ifndef VERTEX_H
#define VERTEX_H


#include <glad/glad.h>
#include <climits>
#include <glm/glm.hpp>

// 顶点
class Vertex
{
public:
	Vertex();
	Vertex(const GLfloat x, const GLfloat y, const GLfloat z);
	~Vertex();
	// 设置x坐标
	int setX(const GLfloat x);
	// 设置y坐标
	int setY(const GLfloat y);
	// 设置z坐标
	int setZ(const GLfloat z);
	// 获得x坐标
	GLfloat getX() const;
	// 获得y坐标
	GLfloat getY() const;
	// 获得z坐标
	GLfloat getZ() const;
	Vertex& operator=(const Vertex& vertex);
	bool operator==(const Vertex& vertex) const;
	bool operator!=(const Vertex& vertex) const;
	operator glm::vec3() const;
	operator glm::vec4() const;
	const static int OPERATION_SUSSESS = 1;
	const static int OPERATION_FAILURE = 0;
	const static int UNDEFINED = INT_MIN;
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};



#endif // !VERTEX_H