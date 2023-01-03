
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "sphere.h"
#include <learnopengl/shader_s.h>
#include "model.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_presscallback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float getdist(sphere& sphere);
bool getdir(sphere& sphere);

float fov = 45.0f;
float s = 1.0f;
const GLfloat PI = 3.14159265358979323846f;
float lastX = 400.0f;
float lastY = 300.0f;
float rX = 400.0f;
float rY = 300.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool rotateflag = true;
bool firstMouse = true;
bool freeview = true;
bool showview = false;
bool controlview = false;
float earthdist = 0;
bool earthdir = false;
float sundist = 0;
bool sundir = false;
float objradius = 0;
bool firstlocal = true;
bool changed = false;
float modelyaw = 0;
float modelpitch = 0;
glm::vec3 rotateaxis = glm::vec3(0,0,1);
glm::vec3 lightPos = glm::vec3(1, 5, 1);
glm::vec3 lightColor = glm::vec3(0.5, 0.5, 0.5);
glm::vec3 rotatevector = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 scalevector = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 transvector = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 transvector_pre = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 modellocation = glm::mat4(1.0f);
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
	glfwSetScrollCallback(window, scroll_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	stbi_set_flip_vertically_on_load(false);
	Shader ourShader("modelshader.vs", "modelshader.fs");

	Model ourModel("resources/objects/nanosuit/nanosuit.obj");

	glm::mat4 model = glm::mat4(1.0f);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Camera viewcamera = Camera(glm::vec3(0, 3.0+20.0f*sin(currentFrame), 20.0 * cos(currentFrame))
			, glm::vec3(0, -1.0f*sin(currentFrame), -1.0 * cos(currentFrame)),
			glm::normalize(glm::vec3(0.0f, 1.0f*cos(currentFrame), 0.0f))
		);

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		ourShader.setVec3("viewPos", mycamera.Pos);
		ourShader.setVec3("light[0].position", lightPos);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setFloat("shininess", 6.0f);
		ourShader.setVec3("light[0].ambient", lightColor);
		ourShader.setVec3("light[0].diffuse", 0.4, 0.4, 0.4);
		ourShader.setVec3("light[0].specular", 1.0, 1.0, 1.0);
		ourShader.setFloat("light[0].constant", 1.0f);
		ourShader.setFloat("light[0].linear", 0.09f);
		ourShader.setFloat("light[0].quadratic", 0.032f);

		ourShader.setVec3("light[1].position", glm::vec3(5, 2, 7));
		ourShader.setVec3("light[1].ambient", lightColor);
		ourShader.setVec3("light[1].diffuse", 0.4, 0.2, 0.3);
		ourShader.setVec3("light[1].specular", 0.8, 1.0, 1.0);
		ourShader.setFloat("light[1].constant", 1.0f);
		ourShader.setFloat("light[1].linear", 0.09f);
		ourShader.setFloat("light[1].quadratic", 0.032f);


		ourShader.setVec3("spotLight.position", mycamera.Pos);
		ourShader.setVec3("spotLight.direction", mycamera.Front);
		ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("spotLight.constant", 1.0f);
		ourShader.setFloat("spotLight.linear", 0.09f);
		ourShader.setFloat("spotLight.quadratic", 0.032f);
		ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view;
		if (freeview)
		{
			view = mycamera.Getview();
		}
		else if (showview)
		{
			view = viewcamera.Getview();
		}
		else if (controlview)
		{
			view = mycamera.Getview();
		}
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		if (firstlocal || changed)
		{
			modellocation = glm::mat4(1.0f);
			modellocation = glm::translate(modellocation, transvector);
			modellocation = glm::rotate(modellocation, glm::radians(modelyaw), glm::vec3(0,1,0));

			if (glm::dot(glm::vec3(sin(modelyaw / 180.0 * 3.14), 0, cos(modelyaw/180.0*3.14)), mycamera.Front) < 0)
			{
				modellocation = glm::rotate(modellocation, glm::radians(modelpitch), glm::vec3(-1, 0, 0));
			}
			else
			{
				modellocation = glm::rotate(modellocation, glm::radians(modelpitch), glm::vec3(1, 0, 0));
			}

			modellocation = glm::scale(modellocation, glm::vec3(s));
			firstlocal = false;
			changed = false;
		}

		ourShader.setMat4("model", modellocation);
		ourModel.Draw(ourShader);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;


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
	float mod = sqrt(pow(mycamera.Front.x, 2) + pow(mycamera.Front.y, 2) + pow(mycamera.Front.z, 2));
	return crossmod / mod;
}

void mouse_presscallback(GLFWwindow* window, int button, int action, int mods)
{
	if (freeview)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			lightColor += glm::vec3(0.1, 0.1, 0.1);
			if (sundir && sundist < 1.5)
			{
				std::cout << "点了太阳" << std::endl;
			}
			if (earthdir && earthdist < 1.0)
			{
				std::cout << "点了地球" << std::endl;
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			lightColor -= glm::vec3(0.1, 0.1, 0.1);
		}
	}
	else if (controlview)
	{
		//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		//{
		//	rotateflag = true;
		//	std::cout << true<< std::endl;
		//}
		//else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		//{
		//	rotateflag = false;
		//}
	}

}

void processInput(GLFWwindow* window)
{
	float cameraspeed = deltaTime * 10.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		freeview = false;
		showview = true;
		controlview = false;
	}

	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		freeview = true;
		showview = false;
		controlview = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		freeview = false;
		showview = false;
		controlview = true;
	}


	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos += cameraspeed * mycamera.Front;
			mycamera.Pos -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector += cameraspeed * mycamera.U;
			transvector -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.U));
			changed = true;
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos += cameraspeed * mycamera.Front;
			mycamera.Pos += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector += cameraspeed * mycamera.U;
			transvector += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
			changed = true;
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos -= cameraspeed * mycamera.Front;
			mycamera.Pos -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector -= cameraspeed * mycamera.U;
			transvector -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
			changed = true;
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (freeview)
		{
			mycamera.Pos -= cameraspeed * mycamera.Front;
			mycamera.Pos += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector -= cameraspeed * mycamera.U;
			transvector += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
			changed = true;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos += cameraspeed * mycamera.Front;
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector += cameraspeed * mycamera.U;
			changed = true;
		}
	}

	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos -= cameraspeed * mycamera.Front;
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector -= cameraspeed * mycamera.U;
			changed = true;
		}
	}

	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
			changed = true;
		}
	}

	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (freeview)

			mycamera.Pos += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		else if (controlview)
		{
			transvector_pre = transvector;
			transvector += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
			changed = true;
		}
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


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	

	if (freeview)
	{

		
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

		up.x = sin(glm::radians(-1.0 * mycamera.Pitch)) * cos(glm::radians(mycamera.Yaw));
		up.y = cos(glm::radians(-1.0 * mycamera.Pitch));
		up.z = sin(glm::radians(-1.0 * mycamera.Pitch)) * sin(glm::radians(mycamera.Yaw));

		//std::cout << front.x << "," << front.y << "," << front.z << std::endl;
		//std::cout << up.x << "," << up.y << "," << up.z << std::endl;
		mycamera.Front = glm::normalize(front);
		mycamera.U = glm::normalize(up);
	}
	else if (controlview)
	{
		if (rotateflag)
		{

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			if (xoffset == 0 && yoffset == 0)
			{
				return;
			}
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.05;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			modelyaw += xoffset;
			modelpitch += yoffset;
			
			changed = true;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (freeview)
	{
		fov -= (float)yoffset;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
	}
	else if (controlview)
	{
		s -= (float)yoffset/45.0;
		if (s < 0.1f)
			s = 0.1f;
		if (s > 10.0f)
			s = 10.0f;
		changed = true;
	}

}