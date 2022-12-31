#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "Object.h"
#include "Plane.h"
#include "Vertex.h"
#include "Scene.h"
#include "StringUtil.h"
#include "Shader.h"

static Scene scene = Scene::getInstance();
static double lastCursorX0 = 0;
static double lastCursorY0 = 0;
static double lastCursorX1 = 0;
static double lastCursorY1 = 0;
static double lastCursorX2 = 0;
static double lastCursorY2 = 0;
static double cameraX = 0;
static double cameraY = 0;
static float level = 25.0F;

// 渲染回调函数（eight.uniform.obj）
static void onRender(GLFWwindow* window, Object& object, glm::vec3& lightPos)
{
	glUseProgram(object.getShaderProgram());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(object.getTransformation()));
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(scene.getCamera().getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(scene.perspective));
	glUniform3f(glGetUniformLocation(object.getShaderProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(object.getShaderProgram(), "lightColor"), 1.0F, 1.0F, 1.0F);
	glUniform3f(glGetUniformLocation(object.getShaderProgram(), "viewPos"), scene.getCamera().getEye().x, scene.getCamera().getEye().y, scene.getCamera().getEye().z);
	glBindVertexArray(object.getVAO());
	glDrawElements(GL_TRIANGLES, object.getPlanes().size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
}

// 渲染回调函数（bezier曲面）
static void onBezierRender(GLFWwindow* window, Object& object, glm::vec3& lightPos)
{
	glUseProgram(object.getShaderProgram());
	glUniform1f(glGetUniformLocation(object.getShaderProgram(), "uOuter0"), level);
	glUniform1f(glGetUniformLocation(object.getShaderProgram(), "uOuter1"), level);
	glUniform1f(glGetUniformLocation(object.getShaderProgram(), "uOuter2"), level);
	glUniform1f(glGetUniformLocation(object.getShaderProgram(), "uOuter3"), level);
	glUniform1f(glGetUniformLocation(object.getShaderProgram(), "uInner0"), level);
	glUniform1f(glGetUniformLocation(object.getShaderProgram(), "uInner1"), level);
	glBindTexture(GL_TEXTURE_2D, object.getTextures()[0].getId());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(object.getTransformation()));
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(scene.getCamera().getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(scene.perspective));
	glUniform3f(glGetUniformLocation(object.getShaderProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(object.getShaderProgram(), "lightColor"), 1.0F, 1.0F, 1.0F);
	glUniform3f(glGetUniformLocation(object.getShaderProgram(), "viewPos"), scene.getCamera().getEye().x, scene.getCamera().getEye().y, scene.getCamera().getEye().z);
	glBindVertexArray(object.getVAO());
	glPatchParameteri(GL_PATCH_VERTICES, object.getVertices().size());
	glPointSize(5.0F);
	glDrawArrays(GL_PATCHES, 0, object.getVertices().size());
	glBindVertexArray(NULL);
}

// 渲染回调函数（bezier曲面的控制点）
static void onBezierPointRender(GLFWwindow* window, Object& object)
{
	glUseProgram(object.getShaderProgram());
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(object.getTransformation()));
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(scene.getCamera().getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(object.getShaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(scene.perspective));
	glBindVertexArray(object.getVAO());
	glPointSize(5.0F);
	glDrawArrays(GL_POINTS, 0, object.getVertices().size());
	glBindVertexArray(NULL);
}

// 按键回调函数
static void onKeyDown(GLFWwindow* window, Object& object, double& lastCursorX, double& lastCursorY)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //重置
	{
		std::cout << "reset" << '\n';
		object.resetTranslation();
		scene.getCamera().setEye(glm::vec3(0, 0, 3));
		scene.getCamera().setCenter(glm::vec3(0, 0, 0));
		scene.getCamera().setUp(glm::vec3(0, 1, 0));
		cameraX = 0;
		cameraY = 0;
		lastCursorX = 0;
		lastCursorY = 0;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //向右平移
	{
		std::cout << "move right" << '\n';
		object.translate(glm::vec3(0.02F, 0.0, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //向左平移
	{
		std::cout << "move left" << '\n';
		object.translate(glm::vec3(-0.02F, 0.0, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //向上平移
	{
		std::cout << "move up" << '\n';
		object.translate(glm::vec3(0, 0.02F, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //向下平移
	{
		std::cout << "move down" << '\n';
		object.translate(glm::vec3(0, -0.02F, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) //向外平移
	{
		std::cout << "move outward" << '\n';
		object.translate(glm::vec3(0, 0, 0.02F));
	}
	else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) //向内平移
	{
		std::cout << "move inward" << '\n';
		object.translate(glm::vec3(0, 0, -0.02F));
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //向上旋转
	{
		std::cout << "self rotate upward" << '\n';
		object.selfRotate(-5.0, glm::vec3(1.0, 0.0, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //向下旋转
	{
		std::cout << "self rotate down" << '\n';
		object.selfRotate(5.0, glm::vec3(1.0, 0.0, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //向左旋转
	{
		std::cout << "self rotate left" << '\n';
		object.selfRotate(-5.0, glm::vec3(0.0, 1.0, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //向右旋转
	{
		std::cout << "self rotate right" << '\n';
		object.selfRotate(5.0, glm::vec3(0.0, 1.0, 0.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //向左旋转
	{
		std::cout << "self rotate left" << '\n';
		object.selfRotate(5.0, glm::vec3(0.0, 0.0, 1.0));
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //向右旋转
	{
		std::cout << "self rotate right" << '\n';
		object.selfRotate(-5.0, glm::vec3(0.0, 0.0, 1.0));
	}
	else //保持原有状态
	{
	}
}

// 鼠标回调函数
static void onMouseAction(GLFWwindow* window, Object& object, double& lastCursorX, double& lastCursorY)
{
	double currentX = 0, currentY = 0;
	glfwGetCursorPos(window, &currentX, &currentY);
	if ((lastCursorX == 0 && lastCursorY == 0) || (lastCursorX == currentX && lastCursorY == currentY))
	{
		lastCursorX = currentX;
		lastCursorY = currentY;
		return;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // 按鼠标左键
	{
		std::cout << "mouse left button" << '\n';
		if (std::abs(currentX - lastCursorX) > 50 || std::abs(currentY - lastCursorY) > 50)
		{
			lastCursorX = currentX;
			lastCursorY = currentY;
			return;
		}
		object.selfRotate(currentX - lastCursorX, glm::vec3(0.0, 1.0, 0.0));
		object.selfRotate(lastCursorY - currentY, glm::vec3(1.0, 0.0, 0.0));
		lastCursorX = currentX;
		lastCursorY = currentY;
		return;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // 按鼠标右键
	{
		std::cout << "mouse right button" << '\n';
		if (std::abs(currentX - lastCursorX) > 50 || std::abs(currentY - lastCursorY) > 50)
		{
			lastCursorX = currentX;
			lastCursorY = currentY;
			return;
		}
		scene.getCamera().setEye(glm::vec3((cameraX + ((currentX - lastCursorX) > 0 ? 0.1 : -0.1)), (cameraY + ((currentY - lastCursorY) > 0 ? 0.1 : -0.1)), 3));
		scene.getCamera().setCenter(glm::vec3(0, 0, 0));
		scene.getCamera().setUp(glm::vec3(0, 1, 0));
		cameraX += (currentX - lastCursorX) > 0 ? 0.1 : -0.1;
		cameraY += (currentY - lastCursorY) > 0 ? 0.1 : -0.1;
		lastCursorX = currentX;
		lastCursorY = currentY;
		return;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	scene.perspective = glm::perspective(glm::radians(45.0F), (GLfloat)width / (GLfloat)height, 0.1F, 1000.F);
}

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	scene.init();
	scene.createWindow(800, 600, "OpenGL Test", NULL, NULL);
	glm::vec3 lightPos(3.0F, 4.0F, 5.0F);
	Object object;
	object.loadObject(*(new std::ifstream("bezier.obj")));
	object.addShader(*new Shader(new std::ifstream("object1.vert.glsl"), GL_VERTEX_SHADER));
	object.addShader(*new Shader(new std::ifstream("object1.frag.glsl"), GL_FRAGMENT_SHADER));
	
	scene.addObject(&object);
	object.draw();
	Object bezierObject;
	bezierObject.loadObject(*(new std::ifstream("bezier.obj")));
	bezierObject.addShader(*new Shader(new std::ifstream("bezier.vert.glsl"), GL_VERTEX_SHADER));
	bezierObject.addShader(*new Shader(new std::ifstream("bezier.frag.glsl"), GL_FRAGMENT_SHADER));
	bezierObject.addShader(*new Shader(new std::ifstream("bezier.tcs.glsl"), GL_TESS_CONTROL_SHADER));
	bezierObject.addShader(*new Shader(new std::ifstream("bezier.tes.glsl"), GL_TESS_EVALUATION_SHADER));
	bezierObject.addTexture(*new Texture(*new std::string("20201110234323.png"), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR));
	scene.addObject(&bezierObject);
	bezierObject.draw();
	Object eight;
	eight.loadObject(*(new std::ifstream("eight.uniform.obj")));
	eight.addShader(*new Shader(new std::ifstream("object2.vert.glsl"), GL_VERTEX_SHADER));
	eight.addShader(*new Shader(new std::ifstream("object2.frag.glsl"), GL_FRAGMENT_SHADER));
	eight.draw();
	scene.addObject(&eight);
	eight.translate(glm::vec3(0.6, 0, 0));
	object.translate(glm::vec3(-0.6, 0, 0));
	bezierObject.translate(glm::vec3(-0.6, 0, 0));
	scene.addOnRender(std::bind(&onBezierPointRender, std::placeholders::_1, std::ref(object)));
	scene.addOnKeyDown(std::bind(&onKeyDown, std::placeholders::_1, std::ref(object), lastCursorX0, lastCursorY0));
	scene.addOnMouseAction(std::bind(&onMouseAction, std::placeholders::_1, std::ref(object), lastCursorX0, lastCursorY0));
	scene.addOnRender(std::bind(&onBezierRender, std::placeholders::_1, std::ref(bezierObject), std::ref(lightPos)));
	scene.addOnKeyDown(std::bind(&onKeyDown, std::placeholders::_1, std::ref(bezierObject), lastCursorX1, lastCursorY1));
	scene.addOnMouseAction(std::bind(&onMouseAction, std::placeholders::_1, std::ref(bezierObject), lastCursorX1, lastCursorY1));
	scene.addOnRender(std::bind(&onRender, std::placeholders::_1, std::ref(eight), std::ref(lightPos)));
	scene.addOnMouseAction(std::bind(&onMouseAction, std::placeholders::_1, std::ref(eight), lastCursorX2, lastCursorY2));
	scene.addOnKeyDown(std::bind(&onKeyDown, std::placeholders::_1, std::ref(eight), lastCursorX2, lastCursorY2));
	scene.perspective = glm::perspective(glm::radians(45.0F), 4.0F / 3.0F, 0.1F, 1000.F);
	scene.getCamera().setEye(glm::vec3(0, 0, 3));
	scene.getCamera().setCenter(glm::vec3(0, 0, 0));
	scene.getCamera().setUp(glm::vec3(0, 1, 0));
	scene.start();

	return 0;
}
