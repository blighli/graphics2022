
#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Utils.h"
#include "Sphere.h"
#include "camera.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

static const int screen_width = 1920;
static const int screen_height = 1080;
int width = 0;
int height = 0;

GLuint renderingProgram = 0;

static const int numberVAOs = 1;
static const int numberVBOs = 3;
GLuint vao[numberVAOs] = { 0 };
GLuint vbo[numberVBOs] = { 0 };

float cameraLocX = 0.f, cameraLocY = 0.f, cameraLocZ = 0.f;
float sunLocX = 0.f, sunLocY = 0.f, sunLocZ = 0.f;
float earthLocX = 0.f, earthLocY = 0.f, earthLocZ = 0.f;
float moonLocX = 0.f, moonLocY = 0.f, moonLocZ = 0.f;
float aspect = 0.f;
int mvLoc = 0;
int projLoc = 0;
float rotAmt = 0.f;

glm::mat4 mMat(1.f), vMat(1.f), pMat(1.f), mvMat(1.f);

//vector<glm::mat4> mvStack;

GLuint sunTextureId = 0, earthTextureId = 0, moonTextureId = 0;

Sphere sun = Sphere(48);
Sphere earth = Sphere(48);
Sphere moon = Sphere(48);

GLboolean keys[1024] = { GL_FALSE };

Camera camera(glm::vec3(0.f, 0.f, 5.f));
GLfloat lastFrame = 0.0f;
GLfloat deltaTime = 0.0f;

GLboolean firstMouse = GL_TRUE;
GLfloat lastX = 0.f;
GLfloat lastY = 0.f;

std::stack<glm::mat4> mvStack;  //ע�ⲻҪ��vsctor<glm::mat4>�����


void setupVertices()
{
	vector<int> ind = sun.getIndices();
	vector<glm::vec3> vert = sun.getVertices();
	vector<glm::vec2> text = sun.getTexCoords();
	vector<glm::vec3> norm = sun.getNormals();
	vector<glm::vec3> tang = sun.getTangents();

	vector<float> pValues;   //����λ��
	vector<float> tValues;   //��������
	vector<float> nValues;   //��������

	int numIndices = sun.getNumIndices();
	for (int i = 0; i < numIndices; i++)
	{
		//pValues.push_back(vert[ind[i]].x);
		pValues.push_back((vert[ind[i]]).x);
		pValues.push_back((vert[ind[i]]).y);
		pValues.push_back((vert[ind[i]]).z);

		tValues.push_back((text[ind[i]]).s);
		tValues.push_back((text[ind[i]]).t);

		nValues.push_back((norm[ind[i]]).x);
		nValues.push_back((norm[ind[i]]).y);
		nValues.push_back((norm[ind[i]]).z);
	}

	glGenVertexArrays(numberVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numberVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	size_t sizeFloat = sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, pValues.size() * sizeof(float), &(pValues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tValues.size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nValues.size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);
}

void press_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS))
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = GL_TRUE;
	}
	else if (action == GLFW_RELEASE)
	{
		//glfwSetWindowShouldClose(window, GL_FALSE);
		keys[key] = GL_FALSE;
	}
}

void key_movement_callback()
{
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void mouse_move_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = GL_FALSE;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xPos, double yPos)
{
	camera.ProcessMouseScroll(xPos);
}

void init(GLFWwindow* window)
{
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(glm::radians(45.f), aspect, 0.01f, 1000.f);
	glfwGetFramebufferSize(window, &width, &height);


	setupVertices();

	sunTextureId = Utils::loadTexture("resource/shining.jpg");
	earthTextureId = Utils::loadTexture("resource/earth.jpg");
	moonTextureId = Utils::loadTexture("resource/moon.jpg");
}


void display(GLFWwindow* window, float currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.4f, 1.f);

	//�ز����٣��������򴰿��Ǻڵģ�������Ⱦ�κζ���
	glUseProgram(renderingProgram);

	GLfloat currentTimeFrame = glfwGetTime();
	deltaTime = currentTimeFrame - lastFrame;
	lastFrame = currentTimeFrame;

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	key_movement_callback();

	//�ƶ��������:���������-cameraZ�������������������
	//vMat = glm::translate(glm::mat4(1.f), glm::vec3(cameraX, cameraY, -cameraZ));
	vMat = camera.GetViewMatrix();
	/*sunLocZ = -6.f;*/
	mMat = glm::translate(glm::mat4(1.f), glm::vec3(sunLocX, sunLocY, sunLocZ));
	mMat = glm::rotate(glm::mat4(1.f), currentTime * 0.1f, glm::vec3(0.f, 1.f, 0.f));
	pMat = glm::perspective(camera.Zoom, (GLfloat)screen_width / (GLfloat)screen_height, 0.01f, 1000.f);

	//�ҳ˹���
	//mvMat = mMat * vMat;   //���������뿪�ӿ�
	mvMat = vMat * mMat;

	//����ͼ����ѹ���ջ
	mvStack.push(vMat);

	//����һ��uniform��������������ֵ��Ҫ���ĵ�uniform������λ����locationָ����location��ֵӦ����glGetUniformLocation��������
	// ��͸�Ӿ����MV �����Ƹ���Ӧ��ͳһ����
	/*ͨ��һ�±�����uniform���εı��������ý�һ�±���ֵ������Ⱦ���ߡ�
	  location : uniform��λ�á�
	  count : ��Ҫ�������ݵ�����Ԫ�ص�����������Ҫ�޸ĵľ����������
	  transpose : ָ��������������(column major)����GL_FALSE������������(row major)����GL_TRUE����
	  value : ָ����count��Ԫ�ص������ָ�롣
	*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// ----------------------  sun
	/*��ջ�����ľ�����һ�ݣ����������ı任��ϣ�Ȼ�����������������µľ��󸱱������ջ�С�
	*/
	mvStack.push(mvStack.top());
	//������ջ�����ľ������ƽ��
	mvStack.top() *= glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
	mvStack.push(mvStack.top());
	//�ڲ��Ƴ�������£����ض�ջ�����ת������X�ᣩ�����á�
	mvStack.top() *= glm::rotate(glm::mat4(1.f), (float)glfwGetTime() * 0.4f, glm::vec3(0.f, 1.f, 0.f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));

	//�󶨵�̫�����㻺����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//ָ������Ⱦʱ����ֵΪ index �Ķ���������������ݸ�ʽ��λ��
	/*Parameters
	index
		ָ��Ҫ�޸ĵĶ������Ե�����ֵ

		size
		ָ��ÿ���������Ե��������������Ϊ1��2��3����4����ʼֵΪ4������ά����position����3����x, y, z����ɣ�����ɫ��4����r, g, b, a����

		type
		ָ��������ÿ��������������͡����õķ��ų�����GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, �� GL_FLOAT����ʼֵΪGL_FLOAT��

		normalized
		ָ����������ʱ���̶�������ֵ�Ƿ�Ӧ�ñ���һ����GL_TRUE������ֱ��ת��Ϊ�̶���ֵ��GL_FALSE����

		stride
		ָ��������������֮���ƫ���������Ϊ0����ô�������Իᱻ���Ϊ�������ǽ���������һ��ġ���ʼֵΪ0��

		pointer
		ָ��һ��ָ�룬ָ�������е�һ���������Եĵ�һ���������ʼֵΪ0��*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//���û����ͨ�ö�����������,����0��������ɫ���е�layout(location = 0���е�0���Ӧ,����λ��
	glEnableVertexAttribArray(0);

	//�󶨵��������껺����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//������������ 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTextureId);

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, sunTextureId);*/

	//�����޳���Ĭ������£������޳��ǹرյ�
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	glDrawArrays(GL_TRIANGLES, 0, sun.getNumIndices());
	mvStack.pop();


	/**********************************   earth           */
	//����һ�ݶ�ջ��������ѹ���ջ
	/*push()���ڶ�ջ��������һ���µ���Ŀ������ͨ�����Ŀǰ�ڶ�ջ�����ľ�����һ�ݣ�
	  ���������ı任��ϣ�Ȼ�����������������µľ��󸱱������ջ�С�
	*/
	//top()���ڲ��Ƴ�������£����ض�ջ�����������á�
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.f), glm::vec3(glm::sin((float)currentTime) * 2.5f, glm::cos((float)currentTime) * 0.4, glm::cos((float)currentTime) * 0.5f));
	mvStack.push(mvStack.top());
	//������̫����ת������Y����ת
	mvStack.top() *= glm::rotate(glm::mat4(1.f), (float)currentTime, glm::vec3(0.f, 1.f, 0.f));
	mvStack.top() *= glm::scale(glm::mat4(1.f), glm::vec3(0.4f));
	//����һ��uniform��������������ֵ��Ҫ���ĵ�uniform������λ����locationָ����location��ֵӦ����glGetUniformLocation��������
	// ��͸�Ӿ����MV �����Ƹ���Ӧ��ͳһ����
	/*ͨ��һ�±�����uniform���εı��������ý�һ�±���ֵ������Ⱦ���ߡ�
	  location : uniform��λ�á�
	  count : ��Ҫ�������ݵ�����Ԫ�ص�����������Ҫ�޸ĵľ����������
	  transpose : ָ��������������(column major)����GL_FALSE������������(row major)����GL_TRUE����
	  value : ָ����count��Ԫ�ص������ָ�롣
	*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//ָ������Ⱦʱ����ֵΪ index �Ķ���������������ݸ�ʽ��λ��
	/*Parameters
	index
		ָ��Ҫ�޸ĵĶ������Ե�����ֵ

		size
		ָ��ÿ���������Ե��������������Ϊ1��2��3����4����ʼֵΪ4������ά����position����3����x, y, z����ɣ�����ɫ��4����r, g, b, a����

		type
		ָ��������ÿ��������������͡����õķ��ų�����GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, �� GL_FLOAT����ʼֵΪGL_FLOAT��

		normalized
		ָ����������ʱ���̶�������ֵ�Ƿ�Ӧ�ñ���һ����GL_TRUE������ֱ��ת��Ϊ�̶���ֵ��GL_FALSE����

		stride
		ָ��������������֮���ƫ���������Ϊ0����ô�������Իᱻ���Ϊ�������ǽ���������һ��ġ���ʼֵΪ0��

		pointer
		ָ��һ��ָ�룬ָ�������е�һ���������Եĵ�һ���������ʼֵΪ0��*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//���û����ͨ�ö�����������,����0��������ɫ���е�layout(location = 0���е�0���Ӧ
	glEnableVertexAttribArray(0);

	//�󶨵������������껺����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTextureId);
	//�����޳���Ĭ������£������޳��ǹرյ�
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, earth.getNumIndices());
	mvStack.pop();
	mvStack.pop();


	/****************************************   moon */
	//����һ�ݶ�ջ��������ѹ���ջ
	/*push()���ڶ�ջ��������һ���µ���Ŀ������ͨ�����Ŀǰ�ڶ�ջ�����ľ�����һ�ݣ�
	  ���������ı任��ϣ�Ȼ�����������������µľ��󸱱������ջ�С�
	*/
	//top()���ڲ��Ƴ�������£����ض�ջ�����������á�
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.f), glm::vec3(glm::sin((float)currentTime) * 3.f, glm::cos((float)currentTime) * 0.8f, glm::cos((float)currentTime) * 3.0f));
	mvStack.push(mvStack.top());
	//�����Ƶ�����ת������Y����ת
	mvStack.top() *= glm::rotate(glm::mat4(1.f), (float)currentTime * 4.f, glm::vec3(0.f, 1.f, 0.f));
	mvStack.top() *= glm::scale(glm::mat4(1.f), glm::vec3(0.2f));
	//����һ��uniform��������������ֵ��Ҫ���ĵ�uniform������λ����locationָ����location��ֵӦ����glGetUniformLocation��������
	// ��͸�Ӿ����MV �����Ƹ���Ӧ��ͳһ����
	/*ͨ��һ�±�����uniform���εı��������ý�һ�±���ֵ������Ⱦ���ߡ�
	  location : uniform��λ�á�
	  count : ��Ҫ�������ݵ�����Ԫ�ص�����������Ҫ�޸ĵľ����������
	  transpose : ָ��������������(column major)����GL_FALSE������������(row major)����GL_TRUE����
	  value : ָ����count��Ԫ�ص������ָ�롣
	*/
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//ָ������Ⱦʱ����ֵΪ index �Ķ���������������ݸ�ʽ��λ��
	/*Parameters
	index
		ָ��Ҫ�޸ĵĶ������Ե�����ֵ

		size
		ָ��ÿ���������Ե��������������Ϊ1��2��3����4����ʼֵΪ4������ά����position����3����x, y, z����ɣ�����ɫ��4����r, g, b, a����

		type
		ָ��������ÿ��������������͡����õķ��ų�����GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_FIXED, �� GL_FLOAT����ʼֵΪGL_FLOAT��

		normalized
		ָ����������ʱ���̶�������ֵ�Ƿ�Ӧ�ñ���һ����GL_TRUE������ֱ��ת��Ϊ�̶���ֵ��GL_FALSE����

		stride
		ָ��������������֮���ƫ���������Ϊ0����ô�������Իᱻ���Ϊ�������ǽ���������һ��ġ���ʼֵΪ0��

		pointer
		ָ��һ��ָ�룬ָ�������е�һ���������Եĵ�һ���������ʼֵΪ0��*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//���û����ͨ�ö�����������,����0��������ɫ���е�layout(location = 0���е�0���Ӧ
	glEnableVertexAttribArray(0);
	//�󶨵������������껺����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, moonTextureId);

	//�����޳���Ĭ������£������޳��ǹرյ�
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, moon.getNumIndices());
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();


	////�ǿպ�
	//GLfloat skyboxVertices[] = {
	//	// Positions          
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,

	//	-1.0f, -1.0f,  1.0f,
	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f,  1.0f,
	//	-1.0f, -1.0f,  1.0f,

	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,

	//	-1.0f, -1.0f,  1.0f,
	//	-1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f, -1.0f,  1.0f,
	//	-1.0f, -1.0f,  1.0f,

	//	-1.0f,  1.0f, -1.0f,
	//	1.0f,  1.0f, -1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	1.0f,  1.0f,  1.0f,
	//	-1.0f,  1.0f,  1.0f,
	//	-1.0f,  1.0f, -1.0f,

	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f, -1.0f,  1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	1.0f, -1.0f, -1.0f,
	//	-1.0f, -1.0f,  1.0f,
	//	1.0f, -1.0f,  1.0f
	//};

	//GLuint skyboxVAO, skyboxVBO;
	//glGenVertexArrays(1, &skyboxVAO);
	//glGenBuffers(1, &skyboxVBO);
	//glBindVertexArray(skyboxVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glBindVertexArray(0);

	//vector<const GLchar*> faces;
	//faces.push_back("resource/right.jpg");
	//faces.push_back("resource/left.jpg");
	//faces.push_back("resource/top.jpg");
	//faces.push_back("resource/bottom.jpg");
	//faces.push_back("resource/front.jpg");
	//faces.push_back("resource/back.jpg");

}

void window_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	aspect = (float)newWidth / (float)newHeight;
	pMat = glm::perspective(glm::radians(45.f), aspect, 0.01f, 1000.f);
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	int width, height;
	unsigned char* image = nullptr;

	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureId;

}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path, GLboolean alpha /* = GL_FALSE */)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);


	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureId;
}

int main()
{
	int glfwState = glfwInit();
	if (glfwState == GLFW_FALSE)
	{
		cout << "GLFW initialize failed,invoke glfwInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Sun earth moon", nullptr, nullptr);
	if (!window)
	{
		cout << "GLFW create window failed,invoke glfwCreateWindow()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, press_key_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//һ��Ҫ��ʼ��glew�������޷�������ɫ�����
	int glewState = glewInit();
	if (glewState != GLEW_OK)
	{
		cout << "GLEW initialize failed,invoke glewInit()......Error file:" << __FILE__ << "......Error line:" << __LINE__ << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);

	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	return 0;
}