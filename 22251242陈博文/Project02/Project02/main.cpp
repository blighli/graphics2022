#define STB_IMAGE_IMPLEMENTATION
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "camera.h"
#include "sphere.h"
#include <learnopengl/shader_m.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_presscallback(GLFWwindow* window, int button, int action, int mods);
float getdist(sphere& sphere);
bool getdir(sphere& sphere);
unsigned int loadTexture(char const* path);

const GLfloat PI = 3.14159265358979323846f;
float lastX = 400.0f;
float lastY = 300.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool firstMouse = true;
bool freeview = true;
float earthdist = 0;
bool earthdir = false;
float sundist = 0;
bool sundir = false;
Camera mycamera;
sphere mysphere;

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
	glfwSetMouseButtonCallback(window, mouse_presscallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader ourshader("shader.vs", "shader.fs");
	Shader sunshader("sunshader.vs", "sunshader.fs");
	Shader mouseshader("mouseshader.vs", "mouseshader.fs");

	std::vector<float> sphereVertices=mysphere.getVertices();
	std::vector<int> sphereIndices=mysphere.getIndices();
	std::vector<float> sphereNormals=mysphere.getNormals();
	std::vector<float> sphereTexcoords=mysphere.getTexCoords();
	/*2-计算球体顶点*/
	//生成球的顶点


	//std::vector<float> vertices;

	
	const int numIndices = mysphere.X_SEGMENTS*mysphere.Y_SEGMENTS*6;
	const int length = numIndices * 5;
	float vertices[19200];
	for (int i = 0; i < numIndices; i++) {		// 把每一个点上的坐标（x,y,z），纹理坐标（s，t），法向量(a,b,c)存储进对应数组
		int index = sphereIndices[i];
		vertices[i * 8 + 0] = sphereVertices[index * 3 + 0];
		vertices[i * 8 + 1] = sphereVertices[index * 3 + 1];
		vertices[i * 8 + 2] = sphereVertices[index * 3 + 2];
		vertices[i * 8 + 3] = sphereTexcoords[index * 2 + 0];
		vertices[i * 8 + 4] = sphereTexcoords[index * 2 + 1];
		vertices[i * 8 + 5] = sphereNormals[index * 3 + 0];
		vertices[i * 8 + 6] = sphereNormals[index * 3 + 1];
		vertices[i * 8 + 7] = sphereNormals[index * 3 + 2];
	}

	
	
	

	sphere sun = sphere(20, 20, glm::vec3(0.0, 0.0, 15.0), 1.5);
	glm::vec3 lightPos=sun.core;

	std::vector<float> sunVertices = sun.getVertices();
	std::vector<int> sunIndices = sun.getIndices();
	std::vector<float> sunNormals = sun.getNormals();
	std::vector<float> sunTexcoords = sun.getTexCoords();

	const int sunnumIndices = sun.X_SEGMENTS * sun.Y_SEGMENTS * 6;

	float sunvertices[12000];
	for (int i = 0; i < sunnumIndices; i++) {		// 把每一个点上的坐标（x,y,z），纹理坐标（s，t），法向量(a,b,c)存储进对应数组
		int index = sunIndices[i];
		sunvertices[i * 5 + 0] = sunVertices[index * 3 + 0];
		sunvertices[i * 5 + 1] = sunVertices[index * 3 + 1];
		sunvertices[i * 5 + 2] = sunVertices[index * 3 + 2];
		sunvertices[i * 5 + 3] = sunTexcoords[index * 2 + 0];
		sunvertices[i * 5 + 4] = sunTexcoords[index * 2 + 1];

	}

	float mouse[18] = {
	0.01f,0.0f,0.0f,      1.0f,1.0f,1.0f,
	0.0f,0.01f,0.0f,      1.0f,1.0f,1.0f,
	0.0f,-0.005f,0.0f,      1.0f,1.0f,1.0f,
	};





	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	unsigned int  sunVAO, VBO1;
	glGenVertexArrays(1, &sunVAO);
	glGenBuffers(1, &VBO1);



	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sunvertices), sunvertices, GL_STATIC_DRAW);
	glBindVertexArray(sunVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	unsigned int  mouseVAO, VBO2;
	glGenVertexArrays(1, &mouseVAO);
	glGenBuffers(1, &VBO2);



	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mouse), mouse, GL_STATIC_DRAW);
	glBindVertexArray(mouseVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);




	unsigned int texture = loadTexture("resources/textures/complex_earth_4k.jpg");
	unsigned int suntexture = loadTexture("resources/textures/sun.jpg");

	ourshader.use();
	ourshader.setInt("material.diffuse", 0);

	sunshader.use();
	sunshader.setInt("Texture", 0);


	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		earthdist = getdist(mysphere);
		earthdir = getdir(mysphere);
		sundist = getdist(sun);
		sundir = getdir(sun);
		//std::cout << earthdist <<std::endl;

		Camera viewcamera = Camera(glm::vec3(20.0 * sin(currentFrame), 0.0f, 20.0 * cos(currentFrame))
			, glm::vec3(-1.0 * sin(currentFrame), 0.0f, -1.0 * cos(currentFrame)),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		processInput(window);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    //glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		ourshader.use();
		ourshader.setVec3("viewPos", mycamera.Pos);

		ourshader.setVec3("light.position", lightPos);
		ourshader.setFloat("material.shininess", 16.0);
		ourshader.setVec3("light.ambient",0.2, 0.2, 0.2);
		ourshader.setVec3("light.diffuse", 0.5, 0.5, 0.5);
		ourshader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		ourshader.setVec3("material.specular", 0.5, 0.5, 0.5);

		ourshader.setFloat("light.constant", 1.0f);
		ourshader.setFloat("light.linear", 0.045f);
		ourshader.setFloat("light.quadratic", 0.0075f);
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
		//glFrontFace(GL_CCW);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, numIndices);

		sunshader.use();

		sunshader.setMat4("projection", projection);
		sunshader.setMat4("view", view);
		sunshader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, suntexture);

		glBindVertexArray(sunVAO);
		//glFrontFace(GL_CCW);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, sunnumIndices);

		mouseshader.use();
		mouseshader.setMat4("projection", projection);
		mouseshader.setMat4("view", view);
		mouseshader.setMat4("model", model);

		mouseshader.setVec3("up", mycamera.U);
		mouseshader.setVec3("cross", glm::normalize(glm::cross(mycamera.Front, mycamera.U)));
		mouseshader.setVec3("center", mycamera.Pos+mycamera.Front);
		glBindVertexArray(mouseVAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);




		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	return 1;

}
bool getdir(sphere& sphere)
{
	glm::vec3 line = sphere.core - mycamera.Pos;
	float res = line.x * mycamera.Front.x + line.y * mycamera.Front.y + line.z * mycamera.Front.z;
	if (res > 0)
		return true;
	else return false;
}
float getdist(sphere& sphere)
{
	glm::vec3 line = sphere.core - mycamera.Pos;
	glm::vec3 crossres = glm::cross(line, mycamera.Front);
	float crossmod = sqrt(pow(crossres.x, 2) + pow(crossres.y, 2) + pow(crossres.z, 2));
	float mod= sqrt(pow(mycamera.Front.x, 2) + pow(mycamera.Front.y, 2) + pow(mycamera.Front.z, 2));
	return crossmod / mod;
}

void mouse_presscallback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		std::cout << "点了" << std::endl;
		if (sundir && sundist < 1.5)
		{
			std::cout << "点了太阳" << std::endl;
		}
		if (earthdir && earthdist < 1.0)
		{
			std::cout << "点了地球" << std::endl;
		}
	}
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

	glm::vec3 up;

	up.x = sin(glm::radians(-1.0*mycamera.Pitch))* cos(glm::radians(mycamera.Yaw));
	up.y = cos(glm::radians(-1.0*mycamera.Pitch));
	up.z = sin(glm::radians(-1.0*mycamera.Pitch))* sin(glm::radians(mycamera.Yaw));

	//std::cout << front.x << "," << front.y << "," << front.z << std::endl;
	//std::cout << up.x << "," << up.y << "," << up.z << std::endl;
	mycamera.Front = glm::normalize(front);
	mycamera.U = glm::normalize(up);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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