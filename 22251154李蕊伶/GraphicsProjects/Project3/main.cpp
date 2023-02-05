#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "stb_image.h"
#include "model.h"
#include "cube.h"

#include <iostream>
#include <string>
#include <map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 objPos(0.0f, -0.8f, 0.2f);

glm::mat4 model(1.0f);

bool isModelTransform = false;

const float MODELMOVESPEED = 20.5f;
const float MODELROTATESPEED = 1.f;
const float MODELSCALESPEED = 1.01f;

using Model_Movement = Camera_Movement;
void ModelProcessKeyboard(Model_Movement direction, float deltaTime);
void ModelProcessRotateXY(float xoffset, float yoffset);
void ModelProcessRotateZ(float offset);
void ModelProcessZoom(enum ZOOM type, float deltaTime);

int main()
{
#pragma region 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	// glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
#pragma endregion

	Model character("./resources/backpack/backpack.obj");
	Cube lightCube;

	Shader lightShader("./light.vs", "./light.fs");
	Shader objShader("./model_loading.vs", "./model_loading.fs");


	vector<glm::vec3> lightPos = {
		glm::vec3(0.0f, 1.0f, 0.5f),
		glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
	};

	vector<glm::vec3> lightColor = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	model = glm::translate(model, objPos);
	model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(glm::mat4(1.0f), objPos) * model;


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// draw character

		objShader.use();
		objShader.setMat4("projection", projection);
		objShader.setMat4("view", view);
		objShader.setMat4("model", model);

		objShader.setVec3("viewPos", camera.Position);
		for (int i = 0; i < lightPos.size(); i++) {
			objShader.setVec3("lightPos[" + std::to_string(i) + "]", lightPos[i]);
			objShader.setVec3("lightColor[" + std::to_string(i) + "]", lightColor[i]);
		}
		character.Draw(objShader);

		// draw lights
		glBindVertexArray(lightCube.ID);
		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		for (int i = 0; i < lightPos.size(); i++) {
			glm::mat4 lightmodel = glm::mat4(1.0f);
			lightmodel = glm::translate(lightmodel, lightPos[i]);
			lightmodel = glm::scale(lightmodel, glm::vec3(0.1f, 0.1f, 0.1f));

			lightShader.setMat4("model", lightmodel);
			lightShader.setVec3("lightColor", lightColor[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void ModelProcessKeyboard(Model_Movement direction, float deltaTime)
{
	float velocity = MODELMOVESPEED * deltaTime;
	glm::vec3 front = camera.Front - camera.Position;
	glm::vec3 up = glm::normalize(camera.Up);
	glm::vec3 right = glm::cross(front, up);
	front = glm::normalize(front);
	if (direction == FORWARD)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f) * velocity);
	if (direction == BACKWARD)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f) * velocity);
	if (direction == LEFT)
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f) * velocity);
	if (direction == RIGHT)
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f) * velocity);
	if (direction == TOP)
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f) * velocity);
	if (direction == DOWN)
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f) * velocity);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		isModelTransform = true;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		isModelTransform = false;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (!isModelTransform) camera.ProcessKeyboard(FORWARD, deltaTime);
		else ModelProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (!isModelTransform) camera.ProcessKeyboard(BACKWARD, deltaTime);
		else ModelProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (!isModelTransform) camera.ProcessKeyboard(LEFT, deltaTime);
		else ModelProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (!isModelTransform) camera.ProcessKeyboard(RIGHT, deltaTime);
		else ModelProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (!isModelTransform)
			camera.ProcessKeyboard(TOP, deltaTime);
		else
			ModelProcessKeyboard(TOP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

		if (!isModelTransform)
			camera.ProcessKeyboard(DOWN, deltaTime);
		else
			ModelProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && isModelTransform)
		ModelProcessZoom(ZOOM_IN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && isModelTransform)
		ModelProcessZoom(ZOOM_OUT, deltaTime);

}

void ModelProcessZoom(enum ZOOM type, float deltaTime) {
	glm::vec3 SCALE = glm::vec3(1.f);
	if (type == ZOOM_IN)
		SCALE *= MODELSCALESPEED;
	else
		SCALE /= MODELSCALESPEED;
	model = glm::scale(model, SCALE);
}

void ModelProcessRotateXY(float xoffset, float yoffset) {
	float rotate_angle_y_axis = MODELROTATESPEED * xoffset;
	float rotate_angle_x_axis = MODELROTATESPEED * yoffset;
	model = glm::rotate(model, glm::radians(rotate_angle_y_axis), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(rotate_angle_x_axis), glm::vec3(1.f, 0.f, 0.f));

}


void ModelProcessRotateZ(float offset) {
	float rotate_angle_z_axis = MODELROTATESPEED * offset * 2.f;
	model = glm::rotate(model, glm::radians(rotate_angle_z_axis), glm::vec3(0.f, 0.f, 1.f));
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	if (!isModelTransform) camera.ProcessMouseMovement(xoffset, yoffset);
	else ModelProcessRotateXY(xoffset, yoffset);
	lastX = xpos;
	lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (!isModelTransform) camera.ProcessMouseScroll(yoffset);
	else ModelProcessRotateZ(yoffset);
}