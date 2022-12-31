#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

class Sphere {
public:
	unsigned int VAO;
	int x_segments;
	int y_segments;

	Sphere(const int X_SEGMENTS, const int Y_SEGMENTS) : x_segments(X_SEGMENTS), y_segments(Y_SEGMENTS) {
		for (int y = 0; y <= Y_SEGMENTS; y++)
		{
			for (int x = 0; x <= X_SEGMENTS; x++)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				sphereVertices.push_back(xPos);
				sphereVertices.push_back(yPos);
				sphereVertices.push_back(zPos);
				sphereVertices.push_back(xSegment);
				sphereVertices.push_back(ySegment);
			}
		}

		//生成球的Indices
		for (int i = 0; i < Y_SEGMENTS; i++)
		{
			for (int j = 0; j < X_SEGMENTS; j++)
			{
				sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
				sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
				sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
				sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
				sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
				sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
			}
		}

		unsigned int VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//生成并绑定球体的VAO和VBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//将顶点数据绑定至当前默认的缓冲中
		glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

		GLuint element_buffer_object;//EBO
		glGenBuffers(1, &element_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

		//设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//解绑VAO和VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
private:
	std::vector<float> sphereVertices;
	std::vector<int> sphereIndices;
	const float PI = 3.1415926;
};
#endif