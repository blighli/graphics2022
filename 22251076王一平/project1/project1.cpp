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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int EBO;
unsigned int VBO;
unsigned int VAO;
glm::vec3 Positions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
};
float seg = 50;


void DrawSphere() {
	// 画球
	std::vector<float> points;
	std::vector<int>indices;

	float PI = std::acos(-1);
	for (int i = 0; i <= seg; i++)
	{
		for (int j = 0; j <= seg; j++) {
			float xse = j / seg;
			float yse = i / seg;
			float x = std::cos(xse * 2.0f * PI) * std::sin(yse * PI);
			float y = std::cos(yse * PI);
			float z = std::sin(xse * 2.0f * PI) * std::sin(yse * PI);
			points.push_back(x);
			points.push_back(y);
			points.push_back(z);
			points.push_back(xse * 1.0f);
			points.push_back(yse * 1.0f);
		}
	}
	for (int i = 0; i < seg; i++)
	{
		for (int j = 0; j < seg; j++)
		{
			indices.push_back(i * ((int)seg + 1) + j);
			indices.push_back((i + 1) * ((int)seg + 1) + j);
			indices.push_back((i + 1) * ((int)seg + 1) + j + 1);
			indices.push_back(i * ((int)seg + 1) + j);
			indices.push_back((i + 1) * ((int)seg + 1) + j + 1);
			indices.push_back(i * ((int)seg + 1) + j + 1);
		}
	}

	//****************************************************************************
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int LoadTexture() {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Textures/Moon.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}



int main(int argc, char** argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Project1", nullptr, nullptr);
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

	// 创建Shader
	Shader shader("Shaders/VertexShader", "Shaders/FragmentShader");
	shader.use();

	// 初始化顶点
	DrawSphere();	
	
	// 加载贴图
	unsigned int texture = LoadTexture();	

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 主循环
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glBindTexture(GL_TEXTURE_2D, texture);
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", view);
		glBindVertexArray(VAO);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Positions[0]);
		float angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, (int)(seg * seg) * 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


