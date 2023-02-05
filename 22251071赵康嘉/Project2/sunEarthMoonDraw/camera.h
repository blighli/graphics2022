#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <iostream>

using namespace std;

//ע��a, b��������ȷ��һ���棬��������ֱ���������c��Ȼ��a, c��������ȷ��һ���棬��������ֱ���������d��
//a, c, d���ഹֱ������һ������ϵ������ά�ռ�����������ȷ��������ϵ������һ�����������������ָ��y�᷽������������ϵ��
//ƫ���Ǻ͸��������ú��������ϵ�޹ء�ֻ�ǻ���ƫ���Ǻ͸�����ȷ���µ������λ�ã�
//ȷ�����ǵķ�������һֱΪ�̶�ֵ����λ���ȣ������䷽��ı��ˣ�3D����������������ά�ģ��г���Ҳ�нǶȣ���
//Ȼ�����µķ�������ȷ�����������ϵ��
//��������������������������������
//��Ȩ����������ΪCSDN������xhm01291212����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https ://blog.csdn.net/xhm01291212/article/details/79175887

// ƫת��ʼ��Ϊ - 90.0�ȣ���ΪƫתΪ0.0�ᵼ�·�������ָ���Ҳ࣬�������������������תһ�㡣
//ŷ���ǣ������ǣ�pitch����ƫ���ǣ�yaw���͹�ת�ǣ�roll����
//GLfloat pitch = 0.0f;            //�����ǣ���X����ת����ͷ���¸���,
//								 //�����ǣ�pitch�������������ϵx����ˮƽ�棨��������ϵ��x��нǣ��������x��λ��ˮƽ��x��֮�ϣ�y�������ᣩʱ��
//								 //������������̧ͷ���������ͷ��
//GLfloat yaw = -90.0f;            //ƫ���ǣ���Y����ת����ͷ����ƫת
//								 //ƫ���ǣ�yaw�������������ϵy����ˮƽ��y��нǣ���ָ��x������ʱΪ����ָ��x������ʱΪ����
//GLfloat roll = 0.0f;             //�����ǣ���Z����ת����ͷ���ҷ���
//								 //��ת�ǣ�roll�������������ϵz����ˮƽ��z��нǣ�����������Ϊ������������Ϊ����

//��������ƶ��ļ�������ѡ�����������Զ���ض��ڴ���ϵͳ�����뷽��
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat PITCH = 0.0f;
const GLfloat YAW = -90.0f;
const GLfloat ROLL = 0.0f;
const GLfloat SPEED = 36.0f;
const GLfloat ZOOM = 45.0f;
const GLfloat SENSITIVITY = 0.25f;//���ŵĿ����̶�

//һ�������camera�࣬���ڴ������벢����OpenGL��ʹ�õ���ӦEuler�ǣ�ŷ���ǣ��������;���
class Camera
{
public:
	//�������
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//ŷ����
	GLfloat Pitch;//����
	GLfloat Yaw;  //ƫ��
	GLfloat Roll; //����
	//���ѡ��
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;//���ƫ����ϵ��
	GLfloat Zoom;
	
	//ʹ����������
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat pitch = PITCH, GLfloat yaw = YAW) : Front(glm::vec3(0.0f, 0.0f, -2.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Pitch = pitch;
		this->Yaw = yaw;
		this->UpdateCameraVectors();
	}
	//ʹ�ñ�������
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat pitch, GLfloat yaw) : Front(glm::vec3(0.0f, 0.0f, -2.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Pitch = pitch;
		this->Yaw = yaw;
		this->UpdateCameraVectors();
	}
	//����ʹ��Eular�Ǻ�LookAt����������ͼ����
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}
	//������κ����Ƽ��̵�����ϵͳ���յ����롣�������������ö����ʽ������������Ӵ���ϵͳ����ȡ��
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime * 0.1;
		if (direction == FORWARD)
		{
			this->Position += this->Front * velocity;
		}
		if (direction == BACKWARD)
		{
			this->Position -= this->Front * velocity;
		}
		if (direction == LEFT)
		{
			this->Position -= this->Right * velocity;
			//this->Position -= glm::cross(this->Front, this->Up) * velocity;
		}
		if (direction == RIGHT)
		{
			this->Position += this->Right * velocity;
			//this->Position += glm::cross(this->Front, this->Up) * velocity;
		}
	}
	//������������ϵͳ���յ����롣��x��y������Ҫƫ��ֵ��
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->MouseSensitivity;
		yOffset *= this->MouseSensitivity;

		this->Yaw += xOffset;  //ƫ����������X��
		this->Pitch += yOffset;//������������Y��

		//ȷ���������ǳ���+-90�ȣ���Ļ���ᷭת
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
			{
				this->Pitch = 89.0f;
			}
			if (this->Pitch < -89.0f)
			{
				this->Pitch = -89.0f;
			}
		}

		//ʹ�ø��µ�Eular�Ƕȸ���ǰ���Һ�������
		this->UpdateCameraVectors();
	}

	//������������¼����յ����롣ֻ��Ҫ���봹ֱ������
	void ProcessMouseScroll(GLfloat zOffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		{
			this->Zoom -= zOffset;
		}
		if (this->Zoom <= 1.0f)
		{
			this->Zoom = 1.0f;
		}
		if (this->Zoom >=45.0f)
		{
			this->Zoom = 45.0f;
		}
	}
protected:
private:
	//������������µģ�Eular�Ƕȼ���ǰ����
	void UpdateCameraVectors()
	{
		//�����µ�ǰ����
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);//attention
		//ͬʱ���¼�����������
		//�淶����������Ϊ�����ĳ���Խ�ӽ�0�����ϻ����²��ҵĴ���Խ�࣬�ƶ��ٶȾ�Խ����
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));  //cross������������������෴
	}
};


#endif // !_CAMERA_H_



//enum Camera_Movement {
//	FORWARD,
//	BACKWARD,
//	LEFT,
//	RIGHT
//};
//
//// Default camera values
//const GLfloat YAW = -90.0f;
//const GLfloat PITCH = 0.0f;
//const GLfloat SPEED = 3.0f;
//const GLfloat SENSITIVTY = 0.25f;
//const GLfloat ZOOM = 45.0f;
//
//
//// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
//class Camera
//{
//public:
//	// Camera Attributes
//	glm::vec3 Position;
//	glm::vec3 Front;
//	glm::vec3 Up;
//	glm::vec3 Right;
//	glm::vec3 WorldUp;
//	// Eular Angles
//	GLfloat Yaw;
//	GLfloat Pitch;
//	// Camera options
//	GLfloat MovementSpeed;
//	GLfloat MouseSensitivity;
//	GLfloat Zoom;
//
//	// Constructor with vectors
//	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
//	{
//		this->Position = position;
//		this->WorldUp = up;
//		this->Yaw = yaw;
//		this->Pitch = pitch;
//		this->updateCameraVectors();
//	}
//	// Constructor with scalar values
//	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
//	{
//		this->Position = glm::vec3(posX, posY, posZ);
//		this->WorldUp = glm::vec3(upX, upY, upZ);
//		this->Yaw = yaw;
//		this->Pitch = pitch;
//		this->updateCameraVectors();
//	}
//
//	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
//	glm::mat4 GetViewMatrix()
//	{
//		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
//	}
//
//	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
//	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
//	{
//		GLfloat velocity = this->MovementSpeed * deltaTime;
//		if (direction == FORWARD)
//			this->Position += this->Front * velocity;
//		if (direction == BACKWARD)
//			this->Position -= this->Front * velocity;
//		if (direction == LEFT)
//			this->Position -= this->Right * velocity;
//		if (direction == RIGHT)
//			this->Position += this->Right * velocity;
//	}
//
//	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
//	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
//	{
//		xoffset *= this->MouseSensitivity;
//		yoffset *= this->MouseSensitivity;
//
//		this->Yaw += xoffset;
//		this->Pitch += yoffset;
//
//		// Make sure that when pitch is out of bounds, screen doesn't get flipped
//		if (constrainPitch)
//		{
//			if (this->Pitch > 89.0f)
//				this->Pitch = 89.0f;
//			if (this->Pitch < -89.0f)
//				this->Pitch = -89.0f;
//		}
//
//		// Update Front, Right and Up Vectors using the updated Eular angles
//		this->updateCameraVectors();
//	}
//
//	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
//	void ProcessMouseScroll(GLfloat yoffset)
//	{
//		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
//			this->Zoom -= yoffset;
//		if (this->Zoom <= 1.0f)
//			this->Zoom = 1.0f;
//		if (this->Zoom >= 45.0f)
//			this->Zoom = 45.0f;
//	}
//
//private:
//	// Calculates the front vector from the Camera's (updated) Eular Angles
//	void updateCameraVectors()
//	{
//		// Calculate the new Front vector
//		glm::vec3 front;
//		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
//		front.y = sin(glm::radians(this->Pitch));
//		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
//		this->Front = glm::normalize(front);
//		// Also re-calculate the Right and Up vector
//		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
//	}
//};
//
//#endif // !_CAMERA_H_
////