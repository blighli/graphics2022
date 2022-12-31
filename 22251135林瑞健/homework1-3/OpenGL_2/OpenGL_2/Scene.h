#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <functional>
#include <vector>

#include "Object.h"
#include "Camera.h"

class Object;

// 按键回调函数
typedef std::function<void(GLFWwindow*)> OnKeyDown;
// 鼠标回调函数
typedef std::function<void(GLFWwindow*)> OnMouseAction;
// 渲染回调函数
typedef std::function<void(GLFWwindow* window)> OnRender;

// 舞台
class Scene
{
public:
	// 投影矩阵（Projection）
	glm::mat4 perspective;
	// 获取实例
	static Scene& getInstance();
	// 初始化
	int init() const;
	// 创建窗口
	int createWindow(int width, int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share);
	// 开始渲染
	void start() const;
	// 添加按键回调函数
	void addOnKeyDown(const OnKeyDown& onKeyDown);
	// 添加渲染回调函数
	void addOnRender(const OnRender& onRender);
	// 添加鼠标回调函数
	void addOnMouseAction(const OnMouseAction& onMouseAction);
	// 清除按键回调函数
	void clearOnKeyDown();
	// 清除渲染回调函数
	void clearOnRender();
	// 清除鼠标回调函数
	void clearOnMouseAction();
	// 添加物体
	void addObject(Object* object);
	// 清除物体
	void clearObject();
	void setCamera(Camera& camera);
	Camera& getCamera();
	const static int OPERATION_SUCCESS = 1;
	const static int OPERATION_FAILURE = -1;
	~Scene();

private:
	static Scene* instance;
	Camera camera;
	GLFWwindow* window;
	std::vector<OnKeyDown> onKeyDowns;
	std::vector<OnRender> onRenders;
	std::vector<Object*> objects;
	std::vector<OnMouseAction> onMouseActions;
	Scene();
};



#endif // !SCENE_H
