
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"



const unsigned int width = 800;
const unsigned int height = 800;



GLfloat vertices[] =
{
	-0.5f, -0.5f, -0.5f,   	0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   	1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   	1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   	0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f
};

GLuint indices[] =
{
	0,1,2,
	3,4,5,

	6,7,8,
	9,10,11,

	12,13,14,
	15,16,17,

	18,19,20,
	21,22,23,

	24,25,26,
	27,28,29,

	30,31,32,
	33,34,35
};

void initGL()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main()
{
	initGL();

	GLFWwindow* window = glfwCreateWindow(width, height, "HW_1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	Shader shaderProgram("shader.vert", "shader.frag");



	//VAO  VBO  EBO
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	//纹理
	std::string texPath = "texture.png";

	Texture Tex(texPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Tex.texUnit(shaderProgram, "tex0", 0);


	glEnable(GL_DEPTH_TEST);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		Tex.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//清理
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	Tex.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}