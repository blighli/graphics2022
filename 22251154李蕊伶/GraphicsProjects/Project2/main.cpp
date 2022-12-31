#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <camera.h>
#include "sphere.h"

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 0.0f, 22.0f));
float lastX = (float)SCR_WIDTH / 2.0f;
float lastY = (float)SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// the sun position
glm::vec3 sunPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 earthPosition;
glm::vec3 moonPosition;

int main()
{
#pragma region 
	//init the GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project2", NULL, NULL);
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

	//disable the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//enable depth test
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

#pragma endregion

	Sphere sphere(64.0f, 32.0f, 1.0f);

	//create VAO,VBO,EBO 
	unsigned int sphereVAO, sphereVBO, sphereEBO, normalVBO, texVBO;
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);

	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(float), sphere.vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.normals.size() * sizeof(float), sphere.normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.texcoords.size() * sizeof(float), sphere.texcoords.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &sphereEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indices.size() * sizeof(int), sphere.indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int sunTexture = loadTexture("textures/Sun.jpg");
	unsigned int moonTexture = loadTexture("textures/Moon.png");
	unsigned int earthTexture = loadTexture("textures/Earth.jpg");


	// active the textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, moonTexture);

	//create the shaders, include the light and the objects
	Shader lightShader("sphere.vs", "sun.fs");
	Shader objectShader("sphere.vs", "object.fs");

	float Dearth_sun = 10;
	float Dearth_moon = 1.5;
	float Dsun = 3, Dearth = 1, Dmoon = 0.2;
	float rotateStep = PI / (32 * 360);
	float my_time = 0;
	glm::vec3 earthRotateAxis = glm::vec3(cos(glm::radians(66.5)), sin(glm::radians(66.5)), 0.0f);
	glm::vec3 sunRotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);

	earthPosition = glm::vec3(Dearth_sun * sin(my_time + PI / 2), 0, Dearth_sun * cos(my_time + PI / 2));
	moonPosition = earthPosition + glm::vec3(Dearth_moon * cos(glm::radians(5.0f)) * sin(my_time * 12), Dearth_moon * sin(glm::radians(5.0f)) * cos(my_time * 12), Dearth_moon * cos(glm::radians(5.0f)) * cos(my_time * 12));

	glm::mat4 model, view, projection;

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, key_callback);
		// delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// press
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);

		//the sun
		lightShader.use();
		//set the shader sampler
		lightShader.setInt("texture1", 0);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, sunPosition);
		model = glm::scale(model, glm::vec3(Dsun));
		model = glm::rotate(model, my_time, sunRotateAxis);
		lightShader.setMat4("model", model);
		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);

		//the earth
		earthPosition = glm::vec3(Dearth_sun * sin(my_time + PI / 2), 0, Dearth_sun * cos(my_time + PI / 2));
		objectShader.use();
		objectShader.setVec3("lightPos", sunPosition);
		objectShader.setInt("texture2", 1);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, earthPosition);
		model = glm::scale(model, glm::vec3(Dearth));
		model = glm::rotate(model, my_time * 4, earthRotateAxis);
		objectShader.setMat4("model", model);
		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);

		//the moon
		moonPosition = earthPosition + glm::vec3(Dearth_moon * cos(glm::radians(5.0f)) * sin(my_time * 12), Dearth_moon * sin(glm::radians(5.0f)) * cos(my_time * 12), Dearth_moon * cos(glm::radians(5.0f)) * cos(my_time * 12));
		objectShader.use();
		objectShader.setVec3("lightPos", sunPosition);
		objectShader.setInt("texture2", 2);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, moonPosition);
		model = glm::scale(model, glm::vec3(Dmoon));
		objectShader.setMat4("model", model);
		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);

		my_time += rotateStep;
		if (my_time > 2 * PI) {
			my_time -= 2 * PI;
		}
		//update the window
		glfwPollEvents();
		//exchange the color buffer
		glfwSwapBuffers(window);

	}
	//release resources
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		glm::vec3 posNow = camera.Position;
		float distance[3] = { glm::length(sunPosition - posNow), glm::length(earthPosition - posNow), glm::length(moonPosition - posNow) };
		float temp[3] = { glm::dot(camera.Front, glm::normalize(sunPosition - posNow)), glm::dot(camera.Front, glm::normalize(earthPosition - posNow)), glm::dot(camera.Front, glm::normalize(moonPosition - posNow)) };
		float dotMin = 0;
		int index = -1;
		for (int i = 0; i < 3; i++) {
			if (temp[i] > 0 && temp[i] > dotMin && distance[i] < 100) {
				dotMin = temp[i];
				index = i;
			}
		}
		if (index == 0) printf("The sun.\n");
		else if (index == 1) printf("The earth.\n");
		else if (index == 2) printf("The moon.\n");
	}
}
void processInput(GLFWwindow* window)
{
	//ESC close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//WASD move the camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
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
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		//bind texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		//generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//set wrap/filter method
		if (format == GL_RGBA) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//release
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}