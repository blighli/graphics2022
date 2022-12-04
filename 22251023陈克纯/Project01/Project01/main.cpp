#include "GL\freeglut.h"
#include "GLFW\glfw3.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 设置窗口宽和高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw初始化及配置
	glfwInit();
	// 设置主次版本号及核心模式（只使用openGl的子集）
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 绑定上下文环境
	glfwMakeContextCurrent(window);
	// 当窗口大小变化时，调用回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// 循环渲染（窗口不会被关闭）
	while (!glfwWindowShouldClose(window))
	{
		// 处理输入
		processInput(window);

		// 使用自定义的颜色清空屏幕
		glClearColor(0.3f, 0.3f, 0.8f, 1.0f);
		// 清空颜色缓冲位
		glClear(GL_COLOR_BUFFER_BIT);

		// 交换颜色缓冲
		glfwSwapBuffers(window);
		// 检查是否有触发事件
		glfwPollEvents();
	}

	// glfwTerminate用于释放之前分配的GLFW资源
	glfwTerminate();
	return 0;
}

// 处理所有输入
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


// 当窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 控制视口，前两个参数为窗口左小角位置，后两个参数是窗口的宽度和高度
	glViewport(0, 0, width, height);
}


