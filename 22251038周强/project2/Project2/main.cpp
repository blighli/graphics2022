#include <iostream>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL/SOIL.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// 窗口尺寸
const GLuint WIDTH = 800, HEIGHT = 600;

// PI
const GLfloat PI = 3.14159265358979323846f;
// 将球横纵划分成50*50的网格
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;

// 按键响应函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// 光标位置
float lastX = (float)WIDTH / 2.0f;
float lastY = (float)HEIGHT / 2.0f;
bool firstMouse = true;

// 用于控制模型
GLfloat Angle_X = 0, Angle_Y = 0, Scale = 45; // Angle用于旋转, Scale用于模拟前后
glm::vec3 Axis_X(1.f, 0.f, 0.f);
glm::vec3 Axis_Y(0.f, 1.f, 0.f);

// 用于控制视角
glm::vec3 MyLookAt(0.f, 0.f, -5.f);
GLfloat Yaw, Pitch;
bool isControl = false;

// 光源的位置
glm::vec3 lightPos(12.0f, 1.0f, 2.0f);

GLFWwindow* window;

void Init()
{
    // 窗口初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 创建一个GLFW窗口对象
    window = glfwCreateWindow(WIDTH, HEIGHT, "Project2", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // 设置回调函数
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouseButton_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // 初始化GLEW
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);
}

void Sphere(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices)
{

    // 生成球的点集
    for (int y = 0; y <= Y_SEGMENTS; y++)
    {
        float ySegment = (float)y / (float)Y_SEGMENTS;
        float yPos = std::cos(ySegment * PI);
        float v = y * 1.0f / Y_SEGMENTS;
        for (int x = 0; x <= X_SEGMENTS; x++)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float u = x * 1.0f / X_SEGMENTS;
            // 坐标
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
            // 法向量
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
            // 纹理坐标
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    //生成球的Indices
    for (int i = 0; i < Y_SEGMENTS; i++)
    {
        for (int j = 0; j < X_SEGMENTS; j++)
        {
            indices.push_back(i * (X_SEGMENTS + 1) + j);
            indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
            indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            indices.push_back(i * (X_SEGMENTS + 1) + j);
            indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            indices.push_back(i * (X_SEGMENTS + 1) + j + 1);
        }
    }
}

void Precess(GLuint& VBO, GLuint& VAO, GLuint& EBO, GLuint& lightVAO)
{
    // 
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    Sphere(vertices, indices);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 绑定VBO与EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 位置坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 法线
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO

    // lightVAO
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 绑定VBO与EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 位置坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind lightVAO
}

int main()
{
    // 初始化
    Init();

    // 创建并编译着色器
    Shader ObjShader("Shader_Obj.vs", "Shader_Obj.fs");
    Shader LightingShader("Shader_Lighting.vs", "Shader_Lighting.fs");

    // 处理顶点
    GLuint VBO, VAO, EBO, lightVAO;
    Precess(VBO, VAO, EBO, lightVAO);

    /*---加载地球纹理---*/
    GLuint earthTexture;
    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    // 设置参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理滤波器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载、创建纹理并生成mipmaps
    int earthWidth, earthHeight;
    unsigned char* earthImage = SOIL_load_image("earth.jpg", &earthWidth, &earthHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, earthWidth, earthHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earthImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(earthImage);
    glBindTexture(GL_TEXTURE_2D, 0); // 解绑定

    /*---加载太阳纹理---*/
    GLuint sunTexture;
    glGenTextures(1, &sunTexture);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    // 设置参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理滤波器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载、创建纹理并生成mipmaps
    int sunWidth, sunHeight;
    unsigned char* sunImage = SOIL_load_image("sun.jpg", &sunWidth, &sunHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sunWidth, sunHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sunImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(sunImage);
    glBindTexture(GL_TEXTURE_2D, 0);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Render
        // 清空颜色缓存
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 纹理载入
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        glUniform1i(glGetUniformLocation(ObjShader.Program, "earthTexture"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        glUniform1i(glGetUniformLocation(LightingShader.Program, "sunTexture"), 1);

        /*----地球----*/
        // 激活着色器
        ObjShader.Use();

        GLint lightColorLoc = glGetUniformLocation(ObjShader.Program, "lightColor");
        GLint lightPosLoc = glGetUniformLocation(ObjShader.Program, "lightPos");
        GLint viewPosLoc = glGetUniformLocation(ObjShader.Program, "viewPos");
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc, MyLookAt[0], MyLookAt[1], MyLookAt[2]);

        // 设置model、view、projection参数
        glm::mat4 model(1.f);
        glm::mat4 view(1.f);
        glm::mat4 projection(1.f);
        model = glm::rotate(model, glm::radians((GLfloat)Angle_X), Axis_X);
        model = glm::rotate(model, glm::radians((GLfloat)Angle_Y), Axis_Y);
        view = glm::translate(view, MyLookAt);
        view = glm::rotate(view, glm::radians((GLfloat)Yaw), Axis_Y);
        view = glm::rotate(view, glm::radians((GLfloat)Pitch), Axis_X);
        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

        // 获取着色器中的参数并传递
        GLint modelLoc = glGetUniformLocation(ObjShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ObjShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ObjShader.Program, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 绘制
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /*----光源----*/
        // 激活着色器
        LightingShader.Use();

        model = glm::mat4(1.f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); 
        // 获取着色器中的参数并传递
        modelLoc = glGetUniformLocation(LightingShader.Program, "model");
        viewLoc = glGetUniformLocation(LightingShader.Program, "view");
        projLoc = glGetUniformLocation(LightingShader.Program, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 绘制
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return 0;
}

// 键盘响应函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP) {
        Angle_X -= 1.f;
    }
    else if (key == GLFW_KEY_DOWN) {
        Angle_X += 1.f;
    }
    else if (key == GLFW_KEY_LEFT) {
        Angle_Y -= 1.f;
    }
    else if (key == GLFW_KEY_RIGHT) {
        Angle_Y += 1.f;
    }

    if (key == GLFW_KEY_W) {
        MyLookAt[1] += 0.1f;
    }
    else if (key == GLFW_KEY_S) {
        MyLookAt[1] -= 0.1f;
    }
    else if (key == GLFW_KEY_A) {
        MyLookAt[0] += 0.1f;
    }
    else if (key == GLFW_KEY_D) {
        MyLookAt[0] -= 0.1f;
    }
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:

            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:

            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            isControl = true;
            break;
        default:
            return;
        }
    else {
        isControl = false;
    }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    MyLookAt[2] += 0.1f * yoffset;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!isControl) return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Yaw += xoffset * 0.1;
    Pitch += yoffset * 0.1;
}