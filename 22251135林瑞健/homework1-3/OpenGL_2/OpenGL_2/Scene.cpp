#include "Scene.h"
#include "Object.h"
#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void defaultOnKeyDown(GLFWwindow* window);
static void defaultOnMouseAction(GLFWwindow* window);

Scene* Scene::instance = NULL;

Scene::Scene()
{
	perspective = glm::mat4(1.0F);
	camera = Camera();
}

Scene::~Scene()
{
	glfwTerminate();
}

Scene& Scene::getInstance()
{
	if (instance == NULL)
	{
		instance = new Scene();
	}
	return *instance;
}

int Scene::init() const
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	return OPERATION_SUCCESS;
}

int Scene::createWindow(int width, int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share)
{
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
	if (window == NULL)
	{
		glfwTerminate();
		return OPERATION_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	this->window = window;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return OPERATION_FAILURE;
	}
	return OPERATION_SUCCESS;
}

void Scene::start() const
{
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// input
		// -----
		try
		{
			for (int i = 0; i < onKeyDowns.size(); i++)
			{
				onKeyDowns[i](window);
			}
			for (int i = 0; i < onMouseActions.size(); i++)
			{
				onMouseActions[i](window);
			}
			for (int i = 0; i < onRenders.size(); i++)
			{
				onRenders[i](window);
			}
		}
		catch (const std::bad_function_call& exception)
		{
			std::cerr << exception.what() << '\n';
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Scene::addOnKeyDown(const OnKeyDown& onKeyDown)
{
	this->onKeyDowns.push_back(onKeyDown);
}

void Scene::addOnRender(const OnRender& onRender)
{
	this->onRenders.push_back(onRender);
}

void Scene::addOnMouseAction(const OnMouseAction& onMouseAction)
{
	this->onMouseActions.push_back(onMouseAction);
}

void Scene::clearOnKeyDown()
{
	onKeyDowns.clear();
}

void Scene::clearOnRender()
{
	onRenders.clear();
}

void Scene::clearOnMouseAction()
{
	onMouseActions.clear();
}

void Scene::addObject(Object* object)
{
	this->objects.push_back(object);
}

void Scene::clearObject()
{
	this->objects.clear();
}

void Scene::setCamera(Camera& camera)
{
	this->camera = camera;
}

Camera& Scene::getCamera()
{
	return camera;
}



static void defaultOnKeyDown(GLFWwindow* window)
{

}

static void defaultOnMouseAction(GLFWwindow* window)
{
}
