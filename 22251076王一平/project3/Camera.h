#pragma once
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

// 以模型为中心的平移旋转缩放
float rotateAngle = 0.0f;
glm::vec3 modelPos(0.0f, 0.0f, 0.0f);
glm::vec3 modelScale(0.2f, 0.2f, 0.2f);

// 摄像机坐标
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool isPressing = false;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (!isPressing)
			firstMouse = true;
		isPressing = true;
	}
	else if (action == GLFW_RELEASE) {
		isPressing = false;
	}
	return;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	// 按下QE键旋转模型
	float rotateSpeed = 40.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		rotateAngle += rotateSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rotateAngle -= rotateSpeed;
	// IJKL键移动模型
	float modelSpeed = 1.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		modelPos[2] += modelSpeed;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		modelPos[0] += modelSpeed;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		modelPos[2] -= modelSpeed;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		modelPos[0] -= modelSpeed;
	// UO键缩放模型
	float scaleSpeed = 0.2 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		if (modelScale[0] > 0.05) {
			modelScale -= glm::vec3(scaleSpeed, scaleSpeed, scaleSpeed);
		}
	}	
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		modelScale += glm::vec3(scaleSpeed, scaleSpeed, scaleSpeed);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!isPressing)
		return;

	// 解决鼠标第一次按下时摄像机的跳动问题
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}
