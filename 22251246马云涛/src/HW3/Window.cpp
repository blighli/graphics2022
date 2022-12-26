#include "Window.h"

#include <cassert>
#include <iostream>

void Window::setupGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
}

void Window::create()
{
	window = glfwCreateWindow(width, height, "HW3", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(0);
	}
}

void Window::setupCallBack()
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

Window::Window()
	: lastX(0), lastY(0), window(nullptr) {}

void Window::Initialize()
{
	setupGLFW();
	create();

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	setupCallBack();
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(window);
}

void Window::NextFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::SetCloseFlag(bool flag)
{
	windowCloseFlag = flag;
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window) || windowCloseFlag;
}

void Window::Close()
{
	glfwTerminate();
}

double Window::GetTime()
{
	return glfwGetTime();
}

GLFWwindow *Window::GetWindow()
{
	return window;
}