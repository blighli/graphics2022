#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	~Camera();
	//设置相机位置
	void setEye(glm::vec3 eye);
	//设置相机朝向的点
	void setCenter(glm::vec3 center);
	//设置朝上的方向
	void setUp(glm::vec3 up);
	//获取相机位置
	glm::vec3 getEye() const;
	//获取相机朝向的点
	glm::vec3 getCenter() const;
	//获取朝上的方向
	glm::vec3 getUp() const;
	//获取视角矩阵
	glm::mat4 getViewMatrix();

private:
	//相机位置
	glm::vec3 eye;
	//相机朝向的点
	glm::vec3 center;
	//朝上的方向
	glm::vec3 up;
	//视角矩阵
	glm::mat4 viewMatrix;
	//视角矩阵是否为最新
	bool updatedView;
};
