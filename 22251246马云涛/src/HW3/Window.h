#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	void Initialize();

	void MakeContextCurrent();
	void NextFrame();
	void SetCloseFlag(bool flag);
	bool ShouldClose();
	void Close();

	double GetTime();
	GLFWwindow *GetWindow();

private:
	friend struct FramebufferSizeCallback;

	GLFWwindow *window;

	int width = 500, height = 500;
	bool firstMouse = true;
	float lastX, lastY;

	bool windowCloseFlag = false;

	void setupGLFW();
	void create();
	void setupCallBack();
};
