#define STB_IMAGE_IMPLEMENTATION
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "camera.h"
#include <learnopengl/shader_s.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(char const* path);

float lastX = 400.0f;
float lastY = 300.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool firstMouse = true;
bool freeview = true;
Camera mycamera;




int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader ourshader("shader.vs", "shader.fs");

	float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture = loadTexture("resources/textures/container2.jpg");
	ourshader.use();
	ourshader.setInt("Texture", 0);

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Camera viewcamera=Camera(glm::vec3(3.0*sin(currentFrame),0.0f,3.0*cos(currentFrame))
		,glm::vec3(-1.0*sin(currentFrame),0.0f,-1.0*cos(currentFrame)),
			glm::vec3(1.0f,1.0,0.0f)
		);

		processInput(window);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourshader.use();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view;
		if (freeview)
		{
			view = mycamera.Getview();
		}
		else
		{
			view = viewcamera.Getview();
		}
		ourshader.setMat4("projection", projection);
		ourshader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		ourshader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	return 1;
}

void processInput(GLFWwindow* window)
{
	float cameraspeed = deltaTime * 10.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		freeview = false;
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		freeview = true;
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos += cameraspeed * mycamera.Front;
			mycamera.Pos -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos += cameraspeed * mycamera.Front;
			mycamera.Pos += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos -= cameraspeed * mycamera.Front;
			mycamera.Pos -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos -= cameraspeed * mycamera.Front;
			mycamera.Pos += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos += cameraspeed * mycamera.Front;
	}

	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos -= cameraspeed * mycamera.Front;
	}

	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
	}

	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
	}

	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos += cameraspeed * mycamera.Up;
	}

	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos -= cameraspeed * mycamera.Up;
	}



}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!freeview)
		return;
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

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mycamera.Yaw += xoffset;
	mycamera.Pitch += yoffset;

	if (mycamera.Pitch > 89.0f)
		mycamera.Pitch = 89.0f;
	if (mycamera.Pitch < -89.0f)
		mycamera.Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(mycamera.Yaw)) * cos(glm::radians(mycamera.Pitch));
	front.y = sin(glm::radians(mycamera.Pitch));
	front.z = sin(glm::radians(mycamera.Yaw)) * cos(glm::radians(mycamera.Pitch));
	mycamera.Front = glm::normalize(front);
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

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}