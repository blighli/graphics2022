#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /// <summary>
    /// 创建窗口的时候，尺寸改成用宏定义，方便后面再用到窗口尺寸
    /// </summary>
    /// <returns></returns>
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //// 创建GL_VERTEX_SHADER一个顶点着色器
    //unsigned int vertexShader;
    //vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //// 把字符串绑定到刚刚创建的对象上然后编译他
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);

    //// 检测有没有编译错误
    //int  success;
    //char infoLog[512];
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}

    //// 创建GL_FRAGMENT_SHADER一个片段着色器
    //unsigned int fragmentShader;
    //fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //// 把字符串绑定到刚刚创建的对象上然后编译他
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);

    //// 检测有没有编译错误
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    //if (!success)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}

    //// 创建好着色器之后，还要创建着色器程序
    //unsigned int shaderProgram;
    //shaderProgram = glCreateProgram();

    //// 创建好shader program后，需要把刚刚创建的顶点着色器和片段着色器链接起来
    //// vertex shaer和fragment shader
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);

    //// 链接完之后用log看看有没有报错
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}

    //// 调用完之后，要把用到的资源删除，后面渲染循环只需要用到shader program了
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

    //float vertices[] = {
    //    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    //         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    //};

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

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

    //unsigned int indices[] = {
    //    // 注意索引从0开始! 
    //    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    //    // 这样可以由下标代表顶点组合成矩形

    //    0, 1, 3, // 第一个三角形
    //    1, 2, 3  // 第二个三角形
    //};

    // 绑定一个VBO和一个VAO

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    //unsigned int EBO;
    //glGenBuffers(1, &EBO);

    // 用glVertexAttribPointer告诉OpenGL如何解析顶点数据
    // 
    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 现在属性没那么多了，刚刚除了位置信息还有顶点输出顺序，现在没有EBO了，直接就输出信息，所以不需要绑定那么多

    //// 绑定EBO，用来处理多个三角形情况下，给出下标顺序的输出情况
    // 
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    ////1. then set the vertex attributes pointers
    ////这里两个3，第一个代表我每个顶点有三个数据，第二个代表长度是三个float，就是读完一个顶点要走过的地址长度
    //// 所以实际工作的时候，会走三步，每步走一个float的长度
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// 现在新的顶点数据，有了两个属性，就要启用两个指针，告诉顶点着色器现在有两个属性了
    //// 位置属性
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// 颜色属性
    //// 现在我们知道最后一个偏移量的作用是什么了，同样的最前面的标号0/1也是我们在这边就定义好的，只是一个名字
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);
    //// 2. use our shader program when we want to render an object
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3. now draw the object

    // 前面完成绑定之后，我们要重新绑定一个0上去，防止我们前面绑定好的VBO和VAO被意外修改
    // 同时我们一般不会主动解绑VAO，即使暂时不再调用VAO了
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // 定义载入图片的长度宽度颜色通道数nrChannels，保存数据到data
    int width, height, nrChannels;
    unsigned char* data = stbi_load("F:/photo/container.jpg", &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);

    // 创建并绑定一个纹理
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 生成纹理
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << width << ' ' << height << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    //生成纹理后释放内存
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 重新加载另一张纹理图片，绑定到新的纹理对象上
    data = stbi_load("F:/photo/awesomeface.png", &width, &height, &nrChannels, 0);
    // 生成纹理
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << width << ' ' << height << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    //生成纹理后释放内存
    stbi_image_free(data);

    // 把绘制调整成线框模式
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.shaderProgram, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUseProgram(shaderProgram);
        //glUniform4f(vertexColorLocation, greenValue / 2, greenValue, greenValue / 3, 1.0f);

        //glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        //unsigned int transformLoc = glGetUniformLocation(ourShader.shaderProgram, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        /// <summary>
        /// 下面是入门6.1坐标变换的内容，从这里开始逐渐进入3D的世界
        /// </summary>
        /// <returns></returns>
        /// 
        /// 要想对每个方块用不同的视角（view）就需要在后面再调用model
        //glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        //unsigned int modelLoc = glGetUniformLocation(ourShader.shaderProgram, "model");
        //unsigned int viewLoc = glGetUniformLocation(ourShader.shaderProgram, "view");

        // pass them to the shaders (3 different ways)
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);
        /*
        setMat4函数就能完成前面的
        unsigned int viewLoc = glGetUniformLocation(ourShader.shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        这两步
        相当于执行了
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        传入"view"和view
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        */

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 把绘制调回普通模式
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
