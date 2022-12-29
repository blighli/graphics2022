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

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
Camera camera(glm::vec3(0.0f, 0.0f, 36.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
const float rotate_angle_step_const = 0.5f;
float rotate_angle_step = rotate_angle_step_const;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "Esc_close" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotate_angle_step += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotate_angle_step -= 0.001f;
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
        rotate_angle_step = rotate_angle_step_const;
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

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    //stbi_set_flip_vertically_on_load(true);//纹理坐标匹配问题
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

const GLfloat PI = 3.14159265358979323846f;
const unsigned int X_SEGMENTS = 60;
const unsigned int Y_SEGMENTS = 60;
std::vector<float> vertices;
std::vector<int> indices;
void sphere() {
    //顶点,索引
    for (int y = 0; y <= Y_SEGMENTS; y++) {
        //位置，y轴朝上
        float yAngle = (float)y / (float)Y_SEGMENTS * PI;//与y轴的夹角[0,PI]
        float yPos = std::cos(yAngle);//[1,-1]
        float yTex = (float)y / (float)Y_SEGMENTS;//[0,1]
        for (int x = 0; x <= X_SEGMENTS;x++) {
            float xAngle = (float)x / (float)X_SEGMENTS * 2.0f * PI;//与x轴的夹角[0,2PI]
            float xPos = std::cos(xAngle) * std::sin(yAngle);//[1,0],[0,-1],[-1,0][0,1] * std::sin(yAngle)
            float zPos = std::sin(xAngle) * std::sin(yAngle);//[0,1],[1,0],[0,-1],[-1,0] * std::sin(yAngle)
            float xTex = (float)x / (float)X_SEGMENTS;//[0,1]
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
            //法向量
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
            //纹理
            vertices.push_back(xTex);
            vertices.push_back(yTex);
        }
    }
    for (int y = 0; y < Y_SEGMENTS; y++) {
        for (int x = 0; x <= X_SEGMENTS;x++) {
            //逆时针正面，顺时针反面，注意顺序
            indices.push_back(y * (X_SEGMENTS + 1) + x);//当前
            indices.push_back(y * (X_SEGMENTS + 1) + x + 1);//当前右
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);//当前上
            indices.push_back(y * (X_SEGMENTS + 1) + x + 1);//当前右
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);//当前右上
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);//当前上
        }
    }
}

int main() {
    //GLFE
    glfwInit();
    //窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project02", NULL, NULL);
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
    glfwSetWindowAspectRatio(window, SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    sphere();

    unsigned int VAO, VBO, EBO;//VAO,vertex array objects,顶点缓冲对象(Vertex Buffer Objects, VBO),索引缓冲对象EBO,IBO
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
    // 法向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //着色器
    Shader lightShader("light.vs", "light.fs");//光源，sun
    Shader lightingShader("lighting.vs", "lighting.fs");//物体，earth，moon

    unsigned int sunTexture = loadTexture("sun.jpg");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    unsigned int earthTexture = loadTexture("earth.jpg");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    unsigned int moonTexture = loadTexture("moon.jpg");
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, moonTexture);

    glm::mat4 model, view, projection;
    glm::vec3 sunPosition(0.0f, 0.0f, 0.0f);
    //实际值差太大
    float sun_d = 6.0f;//float sun_d = 1392000.0f;//太阳直径，km
    float sun_rotate = 25.0f * 24.0f;//float sun_rotate = 25.05f;//太阳自转周期，天，h
    float sun_earth_len = 18.0f;//float sun_earth_len = 149597870.0f;//日地距离，km
    float earth_d = 2.0f;//float earth_d = 12742.0f;//地球直径，km
    float earth_rotate = 1.0f * 24.0f;//地球自转周期，天，h
    float earth_sun_rotate = 365.0f * 24.0f;//float earth_sun_rotate = 365.256363f;//地球公转，天，h
    float moon_earth_len = 6.0f;//float moon_earth_len = 384000.0f;//月地距离，km
    float moon_d = 0.5f;//float moon_d = 3476.28f;//月球直径，km
    float moon_rotate = 27.0f * 24.0f;//float moon_rotate = 27.32f;//月球自转周期，天，h
    float moon_earth_rotate = 27.0f * 24.0f;//float moon_earth_rotate = 27.32f;//月球公转，天，h

    glm::vec3 sun_rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);//太阳绕y轴自转
    glm::vec3 earth_rotate_axis = glm::vec3(cos(glm::radians(66.34)), sin(glm::radians(66.34)), 0.0f);//地球自转轴与黄道面(地球的公转轨道平面)成66.34度夹角
    glm::vec3 moon_rotate_axis = glm::vec3(cos(glm::radians(1.5424)), sin(glm::radians(1.5424)), 0.0f);//月球的轨道平面(白道面)与黄道面保持5.145 396°的夹角，月球自转轴则与黄道面的法线成1.5424°的夹角
    //月亮的自转轴与公转轴平行,地球的自转轴和公转轴不平行
    
    float rotated_angle = 0.0;
    //rotate_angle_step = 20.0f;
    //假设地球每次转20度，太阳每次转(rotated_angle*earth_rotate地球自转周期/sun_rotate太阳自转周期)度

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        rotated_angle += rotate_angle_step;

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        
        // sun
        //glBindTexture(GL_TEXTURE_2D, sunTexture);
        lightShader.use();
        lightShader.setInt("light_texture", 0);
        //MVP
        model = glm::mat4(1.0f);
        model = glm::translate(model, sunPosition);
        model = glm::scale(model, glm::vec3(sun_d));//按太阳等比缩放
        model = glm::rotate(model, glm::radians(rotated_angle * earth_rotate / sun_rotate), sun_rotate_axis);//太阳自转
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // earth
        //glBindTexture(GL_TEXTURE_2D, earthTexture);
        float earth_sun_angle = glm::radians(rotated_angle * earth_rotate / earth_sun_rotate);//地球公转角，弧度
        glm::vec3 earthPosition = glm::vec3(sun_earth_len * sin(earth_sun_angle), 0, sun_earth_len * cos(earth_sun_angle));//地球相对太阳位置
        lightingShader.use();
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", sunPosition);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setInt("obj_texture", 1);
        //MVP
        model = glm::mat4(1.0f);
        model = glm::translate(model, earthPosition);
        model = glm::scale(model, glm::vec3(earth_d));
        model = glm::rotate(model, glm::radians(rotated_angle * earth_rotate /earth_rotate), earth_rotate_axis);//地球自转
        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // moon
        //glBindTexture(GL_TEXTURE_2D, moonTexture);
        float moon_earth_angle = glm::radians(rotated_angle * earth_rotate / moon_earth_rotate);//月球公转角，弧度
        glm::vec3 moon_earth_pos = glm::vec3(moon_earth_len * sin(moon_earth_angle), 0, moon_earth_len * cos(moon_earth_angle));//月球相对地球位置
        glm::vec3 moonPosition = earthPosition + moon_earth_pos;//月球相对太阳位置
        lightingShader.use();
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", sunPosition);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setInt("obj_texture", 2);
        //MVP
        model = glm::mat4(1.0f);
        model = glm::translate(model, moonPosition);
        model = glm::scale(model, glm::vec3(moon_d));
        model = glm::rotate(model, glm::radians(rotated_angle * earth_rotate / moon_rotate), moon_rotate_axis);//月球自转
        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}