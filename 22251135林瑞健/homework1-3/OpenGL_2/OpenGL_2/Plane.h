#ifndef PLANE_H
#define PLANE_H

#include <glad/glad.h>
#include <vector>
#include <climits>
#include "Object.h"

class Object;

// 平面
class Plane
{
public:
	Plane();
	~Plane();
	// 平面的数据
	class Data
	{
	public:
		Data(const Plane& plane);
		Data(const Plane& plane, const GLint vertexIndex, const GLint vertexTextureIndex, const GLint normalVectorIndex);
		~Data();
		// 设置顶点索引
		int setVertexIndex(const GLint index);
		// 设置顶点纹理索引
		int setVertexTextureIndex(const GLint index);
		// 设置顶点法向量索引
		int setNormalVectorIndex(const GLint index);
		// 获取顶点索引
		GLint getVertexIndex() const;
		// 设置顶点纹理索引
		GLint getVertexTextureIndex() const;
		// 设置顶点法向量索引
		GLint getNormalVectorIndex() const;
		const static GLint UNDEFINED = INT_MAX;
	private:
		GLint vertexIndex;
		GLint vertexTextureIndex;
		GLint normalVectorIndex;
		const Plane* plane;
	};
	// 获取平面的数据
	const Data getData(const int index) const;
	// 添加数据
	int addData(Data data);
	// 清除数据
	void clearData();
	// 绑定Object
	void setObject(const Object& object);
	// 获取绑定的Object
	const Object* getObject() const;
	// 获取索引对应的顶点
	const Vertex& getVertex(const GLint index) const;
	Plane& operator=(const Plane& plane);
	bool operator==(const Plane& plane) const;
	bool operator!=(const Plane& plane) const;
	const static int OPERATION_SUSSESS = 1;
	const static int OPERATION_FAILURE = 0;
private:
	std::vector<Data> datas;
	const Object* object;
};



#endif // !PLANE_H