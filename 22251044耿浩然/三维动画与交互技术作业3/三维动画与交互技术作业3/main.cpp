
#include<iostream>
#include <algorithm>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"model.h"

glm::mat4 buildScale(float x, float y, float z)
{
	glm::mat4 scale = glm::mat4(x, 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0,
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return scale;
}


const unsigned int width = 800;
const unsigned int height = 800;

const std::string MODEL_PATH = "Boat.obj";

Model model(MODEL_PATH);

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

	GLFWwindow* window = glfwCreateWindow(width, height, "HW_3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	glm::vec4 lightColor_1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 lightPos_1 = glm::vec3(2.0f, 2.0f, 2.0f);

	glm::vec4 lightColor_2 = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos_2 = glm::vec3(-2.0f, 2.0f, 2.0f);

	Shader shaderProgram("shader.vert", "shader.frag");
	//模型处理
	GLfloat* vertices = new GLfloat[model.v_size];
	std::copy(model.vertices.begin(), model.vertices.end(), vertices);
	GLuint* indices = new GLuint[model.idx_size];
	std::copy(model.indices.begin(), model.indices.end(), indices);


	//VAO  VBO  EBO
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, model.v_size*sizeof(GLfloat));
	EBO EBO1(indices, model.idx_size*sizeof(GLuint));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	//纹理
	std::string texPath = "Boat.png";

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

		glm::vec3 modelPos = glm::vec3(0,0,0);
		glm::mat4 modelModel = buildScale(1.0f, 1.0f, 1.0f);
		modelModel = glm::translate(modelModel, modelPos);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelModel));
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor_1"), lightColor_1.x, lightColor_1.y, lightColor_1.z, lightColor_1.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos_1"), lightPos_1.x, lightPos_1.y, lightPos_1.z);
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor_2"), lightColor_2.x, lightColor_2.y, lightColor_2.z, lightColor_2.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos_2"), lightPos_2.x, lightPos_2.y, lightPos_2.z);

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);


		Tex.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, model.idx_size * sizeof(GLuint) / sizeof(int), GL_UNSIGNED_INT, 0);
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