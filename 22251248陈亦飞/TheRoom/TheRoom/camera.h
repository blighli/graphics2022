#ifndef CAMERA_H
#define CAMERA_H

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <glad\glad.h>

glm::vec3 cameraPos = glm::vec3(-8.36505, 16.3797, -28.6039);
glm::vec3 cameraFront = glm::vec3(0.99824, -0.0593061, 1.13284e-06);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
public:
	//glm::vec3 Pos = glm::vec3(0, 0, 0);
	glm::vec3 Pos = glm::vec3(-4.85027, 0, -13.7127);
	glm::vec3 Front = glm::vec3(-0.99824, 0.0593061, -1.13284e-06);
	glm::vec3 Up= glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 U = glm::vec3(0.0f, 1.0f, 0.0f);
	float Yaw=180.0f;
	float Pitch=0.0f;
	Camera() {}
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
	{
		Pos = pos;
		Front = front;
		Up = up;

	}
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up,float yaw,float pitch)
	{
		Pos = pos;
		Front = front;
		Up = up;
		Yaw = yaw;
		Pitch = pitch;
	}

	glm::mat4 Getview()
	{
		return glm::lookAt(Pos, Pos + Front, Up);
	}

};

#endif

