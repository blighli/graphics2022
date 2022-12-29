#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
float mixValue = 0.2f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
int blockorphere = 0;
bool winagain = true;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "Esc_close" << std::endl;
        glfwSetWindowShouldClose(window, true);
        winagain = false;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        //std::cout << "UP" << std::endl;
        mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue >= 1.0f) {
            //std::cout << "UP_MAX" << std::endl;
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        //std::cout << "DOWN" << std::endl;
        mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f) {
            //std::cout << "DOWN_MIX" << std::endl;
            mixValue = 0.0f;
        }
    }
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //std::cout << "W" << std::endl;
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //std::cout << "S" << std::endl;
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //std::cout << "A" << std::endl;
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //std::cout << "D" << std::endl;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        //std::cout << "J" << std::endl;
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        //std::cout << "K" << std::endl;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        blockorphere = (blockorphere + 1) % 2;
        glfwSetWindowShouldClose(window, true);
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};
void draw_block(GLFWwindow* window) {
    //顶点,标准化设备坐标(Normalized Device Coordinates)
    float vertices[] = {
        // 位置                           // 颜色                     //纹理坐标
        -0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,         0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,         0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,          0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,          1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,         0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 1.0f,         0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,         1.0f, 0.0f, 0.0f,         0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,          0.0f, 1.0f, 0.0f,         1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,           0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,           1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,          0.0f, 1.0f, 0.0f,         0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,         0.0f, 0.0f, 1.0f,         0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,          1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,         0.0f, 1.0f, 0.0f,         1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,         0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,          0.0f, 0.0f, 1.0f,         1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,           1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,          0.0f, 1.0f, 0.0f,         1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,         0.0f, 0.0f, 1.0f,         0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,         1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,          0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,           0.0f, 0.0f, 1.0f,         1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,         0.0f, 1.0f, 0.0f,         1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,          0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,          1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,         0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f, 1.0f,         0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,         1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,          0.0f, 1.0f, 0.0f,         1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,           0.0f, 0.0f, 1.0f,         1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,           1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,          0.0f, 1.0f, 0.0f,         0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,         0.0f, 0.0f, 1.0f,         0.0f, 1.0f
    };

    //VAO,vertex array objects,顶点缓冲对象(Vertex Buffer Objects, VBO),索引缓冲对象EBO,IBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    glBindVertexArray(VAO);//绑定VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //着色器
    Shader ourShader("block.vs", "block.fs");//文件格式无所谓
    ourShader.use();
    
    unsigned int texture1, texture2;
    //纹理1
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    int warp_type[] = { GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER }, warp_t_i = 1;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_type[warp_t_i]);//环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_type[warp_t_i]);//环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//过滤方式
    // 加载并生成纹理
    int width, height, nrChannels;
    //unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    //纹理2
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_type[warp_t_i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_type[warp_t_i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置

    glEnable(GL_DEPTH_TEST);

    //渲染循环(Render Loop)
    while (!glfwWindowShouldClose(window))
    {
        //键盘输入
        processInput(window);
        ourShader.setFloat("mixValue", mixValue);
        //渲染指令
        float timeValue = glfwGetTime();
        glClearColor(cos(timeValue) / 2.0f + 0.5f, sin(timeValue) / 2.0f + 0.5f, 110.0f / 256.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 2*num of triangles);// 绘制物体
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;            
            if (i % 3 == 0)
                angle = glfwGetTime() * 25.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void draw_sphere(GLFWwindow* window) {
    const GLfloat PI = 3.14159265358979323846f;
    const unsigned int X_SEGMENTS = 60;
    const unsigned int Y_SEGMENTS = 60;
    //顶点,索引
    std::vector<float> vertices;
    for (int y = 0; y <= Y_SEGMENTS; y++) {
        for (int x = 0; x <= X_SEGMENTS;x++) {
            //位置，y轴朝上
            float xAngle = (float)x / (float)X_SEGMENTS * 2.0f * PI;//与x轴的夹角
            float yAngle = (float)y / (float)Y_SEGMENTS * PI;//与y轴的夹角
            float xPos = std::cos(xAngle) * std::sin(yAngle);
            float yPos = std::cos(yAngle);
            float zPos = std::sin(xAngle) * std::sin(yAngle);
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
            //颜色
            vertices.push_back(xPos * (xPos + yPos + zPos));
            vertices.push_back(yPos * (xPos + yPos + zPos));
            vertices.push_back(zPos * (xPos + yPos + zPos));
            //纹理
            vertices.push_back((xPos+ zPos) * (xPos + yPos + zPos));
            vertices.push_back((yPos+ zPos) * (xPos + yPos + zPos));
        }
    }
    std::vector<int> indices;
    for (int y = 0; y < Y_SEGMENTS; y++) {
        for (int x = 0; x <= X_SEGMENTS;x++) {
            indices.push_back(y * (X_SEGMENTS + 1) + x);//当前
            indices.push_back(y * (X_SEGMENTS + 1) + x + 1);//当前右
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);//当前上
            indices.push_back(y * (X_SEGMENTS + 1) + x + 1);//当前右
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);//当前上
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);//当前右上
        }
    }

    //VAO,vertex array objects,顶点缓冲对象(Vertex Buffer Objects, VBO),索引缓冲对象EBO,IBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);//使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//复制顶点数组到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //着色器
    Shader ourShader("sphere.vs", "sphere.fs");//文件格式无所谓
    ourShader.use();

    unsigned int texture1, texture2;
    //纹理1
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    int warp_type[] = { GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER }, warp_t_i = 1;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_type[warp_t_i]);//环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_type[warp_t_i]);//环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//过滤方式
    // 加载并生成纹理
    int width, height, nrChannels;
    //unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    unsigned char* data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    //纹理2
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warp_type[warp_t_i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warp_type[warp_t_i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置
    
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //渲染循环(Render Loop)
    while (!glfwWindowShouldClose(window))
    {
        //键盘输入
        processInput(window);
        ourShader.setFloat("mixValue", mixValue);
        //渲染指令
        float timeValue = glfwGetTime();
        glClearColor(cos(timeValue) / 2.0f + 0.5f, sin(timeValue) / 2.0f + 0.5f, 110.0f / 256.0f, 1.0f);
        //glClearColor(1.0f - 126.0f / 256.0f, 1.0f - 12.0f / 256.0f, 1.0f - 110.0f / 256.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 2*num of triangles);// 绘制物体
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDrawElements(GL_TRIANGLES, X_SEGMENTS*Y_SEGMENTS*6, GL_UNSIGNED_INT, 0);
        //MVP
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0)
                angle = glfwGetTime() * 25.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//GL_POINTS
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


int main() {
    //GLFE
    glfwInit();
    //窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project01", NULL, NULL);
    glfwMakeContextCurrent(window);
    //GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    //视口
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    //回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while(winagain) {
        if (blockorphere == 0) {
            draw_block(window);
            glfwSetWindowShouldClose(window, false);
            clock_t now = clock();
            while (clock() - now < 90);//延迟防止计算机反应过快、多次切换又切回
        }
        else if (blockorphere == 1) {
            draw_sphere(window);
            glfwSetWindowShouldClose(window, false);
            clock_t now = clock();
            while (clock() - now < 90);
        }
        else {
            glfwSetWindowShouldClose(window, true);
        }
    }
 
    glfwTerminate();
    return 0;
}