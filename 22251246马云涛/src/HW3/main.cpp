#include <iostream>
#include <cassert>

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Loader/RenderUtil.h"
#include "Loader/Mesh.h"
#include "Loader/Model.h"
#include "Loader/ModelLoader.h"
#include "Loader/Shader.h"
#include "Loader/Texture.h"
#include "Loader/Camera.h"

const float FRAMEDURATION = 1.0 / 30;
int width = 500, height = 500;

bool pressed = false;
bool firstMouse = true;

double lastX = 0.0, lastY = 0.0;
const float mouseSensitivity = 0.5;

float radius = 0.0;
float XDegree = 0.0;
float YDegree = 0.0;
float degSpeed = glm::pi<float>() / 180.0 / 10.0;

int viewControl = 0;

Window window;

Camera camera;

void init();
void draw();
void rendGUI();
void renderScene(Shader shader);

void renderQuad();

int totalTime = 0;

int main()
{
    setlocale(LC_ALL, ".UTF8");

    window.Initialize();
    glfwGetWindowSize(window.GetWindow(), &width, &height);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        assert(0);
        // Failed to initialize GLAD
    }

    glfwSetFramebufferSizeCallback(window.GetWindow(), [](GLFWwindow *window, int w, int h)
                                   { glViewport(0, 0, w, h); width = w; height = h; });
    glfwSetMouseButtonCallback(window.GetWindow(), [](GLFWwindow *window, int button, int action, int mods)
                               {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				pressed = true;
				firstMouse = true;
			}
			else
			{
				pressed = false;
			} });
    glfwSetCursorPosCallback(window.GetWindow(), [](GLFWwindow *window, double xpos, double ypos)
                             {
                                 if (firstMouse)
                                 {
                                     lastX = xpos;
                                     lastY = ypos;
                                     firstMouse = false;
                                 }

                                 if (!pressed)
                                     return;

                                 float xoffset = xpos - lastX;
                                 float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                                 lastX = xpos;
                                 lastY = ypos;

                                if(viewControl == 0)
                                {

                                 XDegree -= xoffset * degSpeed;
                                 YDegree -= yoffset * degSpeed;
                                 camera.Position = glm::vec3(radius * glm::cos(YDegree) * glm::sin(XDegree), radius * glm::sin(YDegree), radius * glm::cos(XDegree) * glm::cos(-YDegree));
                                 camera.Yaw = -90 - glm::degrees(XDegree);
                                 camera.Pitch = -glm::degrees(YDegree);
                                 camera.UpdateCameraVectors();
                                }else
                                {
                                 camera.ProcessMouseMovement(xoffset, yoffset);
                                } });

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 450 core");

    init();

    float currentFrame = 0, lastFrame = 0;
    float deltaTime;

    while (!window.ShouldClose())
    {
        currentFrame = window.GetTime();
        if (!lastFrame)
            lastFrame = currentFrame;
        deltaTime = currentFrame - lastFrame;

        if (deltaTime < FRAMEDURATION)
            continue;
        totalTime++;

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window.GetWindow(), true);

        float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        draw();
        rendGUI();

        lastFrame = currentFrame;
        window.NextFrame();
    }

    return 0;
}

Shader lightShader;

Model testScene;

ShaderLoader shaderLoader;
TextureLoader textureLoader;
ModelLoader modelLoader;

/* Control Value */

struct DirLight
{
    glm::vec3 direction;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    glm::vec3 color;
};

DirLight dirLight;

GLuint baseMatrixUBO;
glm::mat4 lightSpaceMatrix;

void init()
{
    // Base Matrix UBO
    {
        // Uniform Buffer Object
        glGenBuffers(1, &baseMatrixUBO);

        glBindBuffer(GL_UNIFORM_BUFFER, baseMatrixUBO);
        glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4) + 5 * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, baseMatrixUBO, 0, 3 * sizeof(glm::mat4) + 5 * sizeof(glm::vec4));
        glCheckError();
    }

    lightShader = shaderLoader.BuildFromFile("Shader/lightingSurface.vert", "Shader/lightingSurface.frag");
    glCheckError();

    testScene = modelLoader.LoadFromFile("Suzanne.obj", ModelLoadingPrefab::Default, glm::mat4(1.0f), true);
    testScene.GenVAO();

    auto boundingSphere = testScene.data.get()->meshes[0].boundingSphere;
    radius = boundingSphere.radius * glm::tan(glm::radians(67.5f));
    // camera.Position = boundingSphere.center;
    camera.Position.z = radius;

    dirLight.direction = glm::vec3(-1, -1, 0);
    dirLight.diffuse = glm::vec3(0.5, 0.5, 0.5);
    dirLight.specular = glm::vec3(0.5, 0.5, 0.5);
    dirLight.ambient = glm::vec3(0.5, 0.5, 0.5);
    dirLight.color = glm::vec3(1, 1, 1);

    /* GL Init */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void draw()
{
    if (height == 0)
        return;

    glEnable(GL_DEPTH_TEST);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / height, 1.0f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::vec3 viewPos = camera.Position;

    {
        glCheckError();

        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCheckError();

        glBindBuffer(GL_UNIFORM_BUFFER, baseMatrixUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 1, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
        glCheckError();

        // 注意对齐
        // vec3 => offset vec4
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 0, sizeof(glm::vec3), glm::value_ptr(viewPos));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 1, sizeof(glm::vec3), glm::value_ptr(dirLight.direction));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 2, sizeof(glm::vec3), glm::value_ptr(dirLight.ambient));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 3, sizeof(glm::vec3), glm::value_ptr(dirLight.diffuse));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3 + sizeof(glm::vec4) * 4, sizeof(glm::vec3), glm::value_ptr(dirLight.specular));
        glCheckError();

        lightShader.Use();
        glCheckError();

        renderScene(lightShader);

        glCheckError();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rendGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Panel");
    ImGui::Text("View");
    ImGui::RadioButton(u8"Model Center", &viewControl, 0);
    ImGui::RadioButton(u8"View Center", &viewControl, 1);

    ImGui::Text("Light");
    ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.direction), 0.01, -1, 1);
    ImGui::DragFloat3("Ambient", glm::value_ptr(dirLight.ambient), 0.01, 0, 1);
    ImGui::DragFloat3("Diffuse", glm::value_ptr(dirLight.diffuse), 0.01, 0, 1);
    ImGui::DragFloat3("Specular", glm::value_ptr(dirLight.specular), 0.01, 0, 1);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void renderScene(Shader shader)
{
    for (int i = 0; i != testScene.data->meshes.size(); i++)
    {
        const auto &mesh = testScene.data->meshes[i];

        const auto &[buffer, transforms] = testScene.data->meshesTransform[i];

        if (buffer)
        {
            if (!transforms.size())
                continue;

            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), &transforms[0], GL_DYNAMIC_DRAW);

            testScene.material.Apply(shader);
            mesh.material.Apply(shader);

            mesh.Draw(transforms.size());
        }
        else
        {
            // Don't use buffer
            for (int i = 0; i != transforms.size(); i++)
            {
                shader.SetModelMatrix(transforms[i]);

                testScene.material.Apply(shader);
                mesh.material.Apply(shader);

                mesh.Draw();
            }
        }

        glCheckError();
    }
}