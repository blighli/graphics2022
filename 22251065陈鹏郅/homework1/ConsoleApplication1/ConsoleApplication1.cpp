﻿#include <stdio.h>
#include <string>
#include <gl/glut>
#include <assert.h>
#include <fstream>
#include "opengl_math.h"
#include "opengl_pipeline.h"
#include "opengl_camera.h"

#define Window_Width 1024
#define Window_Height 768
using namespace std;

const char* pSVFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

GLuint VBO, IBO;
GLuint gWVPLocation;

Camera* pGameCamera = NULL;
PersProjInfo gPersProjInfo;

bool ReadFile(const char* FileName, string& outFile)
{
    ifstream f(FileName);
    bool ret = FALSE;

    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }
        f.close();
        ret = TRUE;
    }
    else {
        fprintf(stderr, "%s:%d: unable to open file : %s", __FILE__, __LINE__, FileName);
        system("pause");
        exit(1);
    }
    return ret;
}
static void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    static float Scale = 0.0f;

    Scale += 0.1f;

    Pipeline p;
    p.Rotate(0.0f, Scale, 0.0f);
    p.WorldPos(0.0f, 0.0f, 3.0f);

    p.SetCamera(*pGameCamera);
    p.SetPerspectiveProj(gPersProjInfo);

    glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, (const GLfloat*)p.GetWVPTrans());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();

}
static void Keyboard(int key, int x, int y)
{
    pGameCamera->OnKeyboard(key);
}
static void InitializeGlutCallback()
{
    glutDisplayFunc(Render);
    glutIdleFunc(Render);

    //注册键盘回调函数
    glutSpecialFunc(Keyboard);
}

static void CreateVertexBuffer()
{
    Vector3f Vertices[4];
    LoadVector3(Vertices[0], -1.0f, -1.0f, 0.5773f);
    LoadVector3(Vertices[1], 0.0f, -1.0f, -1.5f);
    LoadVector3(Vertices[2], 1.0f, -1.0f, 0.5773f);
    LoadVector3(Vertices[3], 0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

}
static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

}
static void AddShader(GLuint ShaderProgram, const char* ShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);
    if (!ShaderObj) {
        fprintf(stderr, "Error creating shader object");
        system("pause");
        exit(1);
    }

    const GLchar* p[1];
    p[0] = ShaderText;
    GLint Length[1];
    Length[0] = strlen(ShaderText);
    glShaderSource(ShaderObj, 1, p, Length);
    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader object: '%s'", InfoLog);
        system("pause");
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}
static void CompilShaders()
{
    GLuint  ShaderProgram = glCreateProgram();
    if (!ShaderProgram) {
        fprintf(stderr, "Error creating shader program");
        system("pause");
        exit(1);
    }

    string vs, fs;

    if (!ReadFile(pSVFileName, vs)) {
        exit(1);
    }
    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    }

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);


    GLchar ErrorLog[1024] = { 0 };
    GLint Success;
    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'", ErrorLog);
        system("pause");
        exit(1);
    }
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Vaild to the Shader program '%s'", ErrorLog);
        system("pause");
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWVPLocation != 0xFFFFFFFF);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(10, 10);
    glutInitWindowSize(Window_Width, Window_Height);
    glutCreateWindow("Camera Space");

    InitializeGlutCallback();

    Vector3f CameraPos, CameraTarget, CameraUp;
    LoadVector3(CameraPos, 0.0f, 0.0f, -4.0f);
    LoadVector3(CameraTarget, 0.0f, 0.0f, 1.0f);
    LoadVector3(CameraUp, 0.0f, 1.0f, 0.0f);
    pGameCamera = new Camera(Window_Width, Window_Height, CameraPos, CameraTarget, CameraUp);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error '%s'\n", glewGetErrorString(res));
        system("pause");
        return 1;
    }

    printf("GL version %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    CreateVertexBuffer();
    CreateIndexBuffer();

    CompilShaders();

    gPersProjInfo.FOV = 60.0f;
    gPersProjInfo.Height = Window_Height;
    gPersProjInfo.Width = Window_Width;
    gPersProjInfo.zNear = 1.0f;
    gPersProjInfo.zFar = 100.0f;

    glutMainLoop();

    return 0;
}



