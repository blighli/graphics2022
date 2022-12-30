#include<iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// 设置多个光源的参数
void setLight(Shader shader)
{
	shader.setVec3("SunLight.direction", 0.0, -1.0, 0.0);
	shader.setVec3("SunLight.color", 1.0, 0.96, 0.84);
	shader.setFloat("SunLight.Ka", 0.1);
	shader.setFloat("SunLight.Kd", 1.0);
	shader.setFloat("SunLight.Ks", 0.7);
	shader.setFloat("SunLight.shininess", 100);

	shader.setVec3("EastLight.direction", -1.0, 0.0, 0.0);
	shader.setVec3("EastLight.color", 0.98, 0.98, 0.98);
	shader.setFloat("EastLight.Ka", 0.1);
	shader.setFloat("EastLight.Kd", 0.5);
	shader.setFloat("EastLight.Ks", 0.5);
	shader.setFloat("EastLight.shininess", 500);

	shader.setVec3("WestLight.direction", 1.0, 0.0, 0.0);
	shader.setVec3("WestLight.color", 0.0, 0.0, 1.0);
	shader.setFloat("WestLight.Ka", 0.1);
	shader.setFloat("WestLight.Kd", 0.5);
	shader.setFloat("WestLight.Ks", 0.5);
	shader.setFloat("WestLight.shininess", 500);

	shader.setVec3("SouthLight.direction", 0.0, 0.0, -1.0);
	shader.setVec3("SouthLight.color", 0.98, 0, 0);
	shader.setFloat("SouthLight.Ka", 0.1);
	shader.setFloat("SouthLight.Kd", 0.5);
	shader.setFloat("SouthLight.Ks", 0.5);
	shader.setFloat("SouthLight.shininess", 500);

	shader.setVec3("NorthLight.direction", 0.0, 0.0, 1.0);
	shader.setVec3("NorthLight.color", 0.98, 0.98, 0.98);
	shader.setFloat("NorthLight.Ka", 0.1);
	shader.setFloat("NorthLight.Kd", 0.5);
	shader.setFloat("NorthLight.Ks", 0.5);
	shader.setFloat("NorthLight.shininess", 500);
}

void paintModel(Model model, Shader shader)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	shader.use();
	setLight(shader);
	model.Draw(shader);
	glPopMatrix();
}


int main(int argc, char** argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Project3", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	GLenum err = glewInit();

	Shader shader = Shader("Shaders/ModelVS.vs", "Shaders/ModelFS.fs");
	vector<glm::vec3> modelPosVec{ glm::vec3(0.0f, 0.0f, 0.0f) };
	Model model = Model("Models/nanosuit/nanosuit.obj", modelPosVec);

	

	// 主循环
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", view);
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::rotate(modelMat, glm::radians(rotateAngle) , glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::translate(modelMat, modelPos);
		modelMat = glm::scale(modelMat, modelScale);
		shader.setMat4("model", modelMat);

		// 绘制模型并设置光照
		paintModel(model, shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


