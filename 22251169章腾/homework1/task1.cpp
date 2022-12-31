//
// Created by zhangteng on 2022/12/25.
//
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "camera.h"
#include "shader.h"

using namespace std;
using namespace glm;
//正方体定义
float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
};
//索引 未采用，直接定义六个面
unsigned int indices[] = {

};

//帧时间控制
float deltaTime = 0.0f;	// 两帧间隔
float lastFrame = 0.0f; // 上一帧时间

// 借助camera类实现相机控制
Camera camera(vec3(0.0f, 0.0f, 5.0f));
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;


//控制相机移动和esc退出
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// 鼠标回调，移动摄像头视角
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// 鼠标滑轮回调，缩放控制
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//将一些初始化进行二次封装
GLFWwindow* init(){
    //初始化
    glfwInit();
    //设置版本为opengl3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //设置为核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //设置窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "task1", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    //绑定一系列外部硬件事件
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //初始化glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);
    return window;
}




int main(){
    //初始化
    GLFWwindow* window= init();
    glEnable(GL_DEPTH_TEST);
    //编译shader，借助shader类编译，不知道啥原因，写相对路径无法识别
    //无奈采用绝对路径方式
    Shader aShader("C:\\development\\sourcecode\\untitled2\\shader\\homework1.vs","C:\\development\\sourcecode\\untitled2\\shader\\homework1.fs");
    unsigned int VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //绑定VAO
    glBindVertexArray(VAO);
    //绑定顶点
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //链接顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);





    while(!glfwWindowShouldClose(window))
    {
        //获得每一帧的时间
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //接收按键操作
        processInput(window);
        //清除缓存
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //使用shader
        aShader.use();
        // 透视和投影
        mat4 projection = perspective(radians(camera.Zoom), (float)(800.0f / 600.0f), 0.1f, 100.0f);
        aShader.setMat4("projection", projection);
        mat4 view = camera.GetViewMatrix();
        aShader.setMat4("view", view);
        mat4 model = mat4(1.0f);
        model = rotate(model, radians(45.0f),vec3(0.0f, 0.0f, 1.0f));
        model = rotate(model,(float)glfwGetTime(),vec3(0.0f, 0.0f, 1.0f));
        model = rotate(model,(float)glfwGetTime(),vec3(0.0f, 1.0f, 0.0f));
        model = rotate(model,(float)glfwGetTime(),vec3(1.0f, 0.0f, 0.0f));
        aShader.setMat4("model", model);
        //使用VAO
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

//加载shader，并返回ID，废弃
//int loadShader(const char* vertexPath, const char* fragmentPath){
//    int ID;
//    string vertexCode;
//    string fragmentCode;
//    ifstream vShaderFile;
//    ifstream fShaderFile;
//    vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
//    fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
//    try
//    {
//        vShaderFile.open(vertexPath);
//        fShaderFile.open(fragmentPath);
//        std::stringstream vShaderStream, fShaderStream;
//        vShaderStream << vShaderFile.rdbuf();
//        fShaderStream << fShaderFile.rdbuf();
//        vShaderFile.close();
//        fShaderFile.close();
//        vertexCode   = vShaderStream.str();
//        fragmentCode = fShaderStream.str();
//    }
//    catch (std::ifstream::failure& e)
//    {
//        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
//    }
//    const char* vShaderCode = vertexCode.c_str();
//    const char * fShaderCode = fragmentCode.c_str();
//    unsigned int vertex, fragment;
//    vertex = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertex, 1, &vShaderCode, NULL);
//    glCompileShader(vertex);
//    fragment = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragment, 1, &fShaderCode, NULL);
//    glCompileShader(fragment);
//    ID = glCreateProgram();
//    glAttachShader(ID, vertex);
//    glAttachShader(ID, fragment);
//    glLinkProgram(ID);
//    //删除着色器
//    glDeleteShader(vertex);
//    glDeleteShader(fragment);
//    return ID;
//}