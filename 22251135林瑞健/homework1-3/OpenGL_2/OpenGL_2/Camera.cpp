#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"

Camera::Camera()
{
	eye = glm::vec3(0, 0, 0);
	center = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);
	updatedView = false;
}

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;
	updatedView = false;
}

Camera::~Camera()
{
}

void Camera::setEye(glm::vec3 eye)
{
	this->eye = eye;
	updatedView = false;
}

void Camera::setCenter(glm::vec3 center)
{
	this->center = center;
	updatedView = false;
}

void Camera::setUp(glm::vec3 up)
{
	this->up = up;
	updatedView = false;
}

glm::vec3 Camera::getEye() const
{
	return eye;
}

glm::vec3 Camera::getCenter() const
{
	return center;
}

glm::vec3 Camera::getUp() const
{
	return up;
}

glm::mat4 Camera::getViewMatrix()
{
	if (!updatedView)
	{
		viewMatrix = glm::lookAt(eye, center, up);
		updatedView = true;
	}
	return viewMatrix;
}
