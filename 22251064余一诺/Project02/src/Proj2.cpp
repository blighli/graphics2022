#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>


// Globals
bool animation = false;

// Timing
float lastFrame = 0.0f;
float frameToggled = 0.0f;
float timeSinceLastToggle = 1.0f;

// Some settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

float earthOrbitRadius = 100.0f;
float moonOrbitRadius = 20.0f;
glm::vec3 sunPos = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float cameraOrbitRadius = 30.0f;
float rotateAngle = 1.0f;

// Mouse Input
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float deltaTime);

int main()
{
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project 2", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader planetShader("src\\planet.vs", "src\\planet.fs");
    Shader sunShader("src\\sun.vs", "src\\sun.fs");

    // Load the models
    Model Sun("models\\Planet\\planet.obj");
    Model Earth("models\\Earth\\Globe.obj");
    Model Moon("models\\Rock\\rock.obj");


    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        timeSinceLastToggle += deltaTime;
        if (animation)
            frameToggled += deltaTime;


        processInput(window, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // view / projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);


        // Render the sun object
        sunShader.use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);

        model = glm::translate(model, sunPos);
        sunShader.setMat4("model", model);
        Sun.Draw(sunShader);


        planetShader.use();

        // Set the lighting
        planetShader.setVec3("viewPos", camera.Position);
        planetShader.setFloat("material.shininess", 32.0f);

        planetShader.setVec3("light.position", sunPos);
        planetShader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);
        planetShader.setVec3("light.diffuse", 1.8f, 1.8f, 1.8f);
        planetShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[0].constant", 1.0f);
        planetShader.setFloat("pointLights[0].linear", 0.045);
        planetShader.setFloat("pointLights[0].quadratic", 0.0075);


        // Render the Earth object
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

        earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);
        model = glm::translate(model, earthPos);
        model = glm::rotate(model, frameToggled * 1.5f * glm::radians(-50.0f), glm::vec3(0.1f, 1.0f, 0.0f));

        planetShader.setMat4("model", model);
        Earth.Draw(planetShader);


        // Render the Moon object
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

        glm::vec3 moonPos = earthPos + glm::vec3(0.0f, sin(frameToggled) * moonOrbitRadius, cos(frameToggled) * moonOrbitRadius);
        model = glm::translate(model, moonPos);
        planetShader.use();
        planetShader.setMat4("model", model);
        Moon.Draw(planetShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, float deltaTime)
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}