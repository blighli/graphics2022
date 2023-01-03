
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "sphere.h"
#include <learnopengl/shader_s.h>
#include "model.h"
#include <iostream>




struct line
{
	float x1;
	float z1;
	float x2;
	float z2;
	bool operator==(const line& l)const
	{
		return (this->x1 == l.x1 && this->z1 == l.z1 && this->x2 == l.x2&&this->z2 == l.z2) || 
			(this->x1 == l.x2 && this->z1 == l.z2 && this->x2 == l.x1 && this->z2 == l.z1);
	}
	bool operator<(const line& l)const
	{
		if (this->x1 < l.x1)
		{
			return true;
		}
		else if (this->x1 == l.x1 && this->x2 < l.x2)
		{
			return true;
		}
		else if (this->x1 == l.x1 && this->x2 == l.x2 && this->z1 < l.z1)
		{
			return true;
		}
		else if (this->x1 == l.x1 && this->x2 == l.x2 && this->z1 == l.z1 && this->z2 == l.z2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_presscallback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
bool wallcollision(glm::vec3 p, glm::vec3 p1);
glm::vec3 getwallline(glm::vec3 p, glm::vec3 p1);
bool crosswall(glm::vec3 p1, glm::vec3 p2);


map<line, int> visited;
float fov = 45.0f;
float s = 1.0f;
const GLfloat PI = 3.14159265358979323846f;
float lastX = 400.0f;
float lastY = 300.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool rotateflag = false;
bool firstMouse = true;
bool freeview = true;
bool showview = false;
bool controlview = false;
bool gameover = false;
bool hold = false;
float earthdist = 0;
bool earthdir = false;
float sundist = 0;
bool sundir = false;
float objradius = 0;
bool firstmonster = true;
bool secondmonster = false;
bool thirdmonster = false;

bool m2boot = false;
int rotatecount2 = 0;
bool m3boot = false;
int rotatecount3 = 0;


glm::mat4 monsterpos2 = glm::mat4(1.0f);//2.23956,0,1.35136
glm::mat4 monsterpos1 = glm::mat4(1.0f);
glm::mat4 monsterpos3 = glm::mat4(1.0f);
glm::mat4 headpos = glm::mat4(1.0f);


glm::vec3 rotateaxis = glm::vec3(0, 0, 1);
glm::vec3 lightPos = glm::vec3(1, 5, 1);
glm::vec3 lightColor = glm::vec3(0.5, 0.5, 0.5);
glm::vec3 rotatevector = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 scalevector = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 transvector = glm::vec3(0.0f, 0.0f, 0.0f);
Camera mycamera;
sphere mysphere;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_presscallback);
	glfwSetScrollCallback(window, scroll_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	stbi_set_flip_vertically_on_load(false);
	Shader ourShader("roomshader.vs", "roomshader.fs");
	Shader monsterShader("monstershader.vs", "monstershader.fs");


	Model ourModel("resources/objects/room/room.obj");

	Model monster1("resources/objects/room/monster1.obj");
	Model monster2("resources/objects/room/monster1.obj");
	Model monster3("resources/objects/room/monster1.obj");
	Model head("resources/objects/room/head.obj");
	glm::mat4 model = glm::mat4(1.0f);

	
	for (int i = 2; i <= 4; i++)
	{
		for (int j = 0; j < ourModel.meshes[i].indices.size()-1; j = j + 3)
		{
			int index0 = ourModel.meshes[i].indices[j];
			int index1 = ourModel.meshes[i].indices[j+1];
			int index2 = ourModel.meshes[i].indices[j+2];

			glm::vec3 v0 = ourModel.meshes[i].vertices[index0].Position;
			glm::vec3 v1 = ourModel.meshes[i].vertices[index1].Position;
			glm::vec3 v2 = ourModel.meshes[i].vertices[index2].Position;

			v0.y = 0;
			v1.y = 0;
			v2.y = 0;

			float d1 = glm::length(v0- v1);
			line newline;
			if (d1 < 1e-5)
			{
				newline.x1 = v0.x;
				newline.z1 = v0.z;
				newline.x2 = v2.x;
				newline.z2 = v2.z;
			}
			else
			{
				newline.x1 = v0.x;
				newline.z1 = v0.z;
				newline.x2 = v1.x;
				newline.z2 = v1.z;
			}
			bool flag = false;
			for (auto it = visited.begin(); it != visited.end(); it++)
			{
				if (it->first == newline)
				{
					flag=true;
					break;
				}
			}
			if (!flag)
			{
				visited[newline] = 1;
			}


		}
	}

	 m2boot = false;
	 rotatecount2 = 0;
	 m3boot = false;
	 rotatecount3 = 0;

	glm::vec3 location1 = glm::vec3(-8.54481, 0, -13.6378);
	glm::vec3 location2 = glm::vec3(-6.54106, 0, -6.38201);
	glm::vec3 location3 = glm::vec3(6.17501, 0, 12.6247);
	glm::vec3 headlocation = glm::vec3(-7.0f, 0.0f, -7.0f);


	headpos = glm::translate(headpos, headlocation);
	headpos = glm::rotate(headpos, glm::radians(0.0f), glm::vec3(0, 1, 0));

	monsterpos2 = glm::translate(monsterpos2, location2);
	monsterpos2 = glm::rotate(monsterpos2, glm::radians(180.0f), glm::vec3(0, 1, 0));
	
	monsterpos1 = glm::translate(monsterpos1, location1);
	monsterpos1 = glm::rotate(monsterpos1, glm::radians(90.0f), glm::vec3(0, 1, 0));
	

	monsterpos3 = glm::translate(monsterpos3, location3);
	monsterpos3 = glm::rotate(monsterpos3, glm::radians(90.0f), glm::vec3(0, 1, 0));

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------

		if (firstmonster&&!gameover)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(p - location1);
			if (dist < 1)
			{
				gameover = true;
				std::cout << "game over" << endl;
			}
		}
		if (secondmonster && !gameover)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(p - location2);
			if (dist < 1)
			{
				gameover = true;
				std::cout << "game over" << endl;
			}
		}
		if (thirdmonster && !gameover)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(p - location3);
			if (dist < 1)
			{
				gameover = true;
				std::cout << "game over" << endl;
			}
		}
		vector<glm::vec3> f_point;
		for (auto it = visited.begin(); it != visited.end(); it++)
		{
			glm::vec3 p1 = glm::vec3(it->first.x1, 0, it->first.z1);
			glm::vec3 p2 = glm::vec3(it->first.x2, 0, it->first.z2);

			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist1 = glm::length(p - p1);
			float dist2 = glm::length(p - p2);

			glm::vec3 f = mycamera.Front;
			f.y = 0;
			f = glm::normalize(f);

			float cos1 = glm::dot(f, (p1 - p)) / (dist1);
			float cos2 = glm::dot(f, (p2 - p)) / (dist2);
			if (dist1 < 6&&cos1>0.8)
			{
				for (int i = 0; i < f_point.size(); i++)
				{
					if (abs(p1.x - f_point[i].x)<1e-4 && abs(p1.z - f_point[i].z)<1e-4)
					{
						break;
					}
					if (i == f_point.size() - 1)
					{
						f_point.push_back(p1);
					}
				}
				if(f_point.size()==0)
					f_point.push_back(p1);
				//std::cout << "dist:" << dist1 << "," << "cos:" << cos1 << endl;
				//std::cout << "--------------------" << endl;
			}
			if (dist2 < 6 && cos2>0.8)
			{
				for (int i = 0; i < f_point.size(); i++)
				{
					if (abs(p2.x - f_point[i].x)<1e-4 && abs(p2.z - f_point[i].z)<1e-4)
					{
						break;
					}
					if (i == f_point.size() - 1)
					{
						f_point.push_back(p2);
					}
				}
				if (f_point.size() == 0)
					f_point.push_back(p2);
				//std::cout << "dist:" << dist2 << "," << "cos:" << cos2 << endl;
				//std::cout << "--------------------" << endl;
			}
		}
		if (f_point.size() != 0)
		{
			//std::cout << "stop" << endl;
		}

		if (mycamera.Pos.z > -5.7&&rotatecount2!=50)
		{
			m2boot = true;
		}
		if (m2boot)
		{
			rotatecount2++;
			monsterpos2 = glm::mat4(1.0f);
			monsterpos2 = glm::translate(monsterpos2, location2);
			monsterpos2 = glm::rotate(monsterpos2, glm::radians(180.0f-(180.0f-90.0f)*(float)rotatecount2/50.0f), glm::vec3(0, 1, 0));
			if (rotatecount2 == 50)
			{
				m2boot = false;
				secondmonster = true;
			}
		}

		if (mycamera.Pos.z > 10.26 && rotatecount3 != 50)
		{
			m3boot = true;
		}
		if (m3boot)
		{
			rotatecount3++;
			monsterpos3 = glm::mat4(1.0f);
			monsterpos3 = glm::translate(monsterpos3, location3);
			monsterpos3 = glm::rotate(monsterpos3, glm::radians(90.0f - (180.0f) * (float)rotatecount3 / 50.0f), glm::vec3(0, 1, 0));
			if (rotatecount3 == 50)
			{
				m3boot = false;
				thirdmonster = true;
			}
		}
		if (firstmonster)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(location1 - p);

			glm::vec3 f = mycamera.Front;
			f.y = 0;
			f = glm::normalize(f);

			float cosine = glm::dot(location1 - p, f) / dist;
			if (dist > 6||dist<=6&&cosine<0.8||crosswall(p,location1))
			{
				glm::vec3 res = getwallline(location1, location1+glm::normalize(p - location1) * glm::vec3(0.05));
				if (res != glm::vec3(0.0f))
				{
					location1=location1+ glm::normalize(res) * glm::vec3(0.05);
					float cosine = glm::dot(glm::normalize(res), glm::vec3(0, 0, 1));
					float angle = acos(cosine);
					monsterpos1 = glm::mat4(1.0f);
					monsterpos1 = glm::translate(monsterpos1, location1);
					if (f.x < 0)
					{
						monsterpos1 = glm::rotate(monsterpos1, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
					}
					else
					{
						monsterpos1 = glm::rotate(monsterpos1, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
					}
				}
				else
				{
					location1 = location1 + glm::normalize(p - location1) * glm::vec3(0.05);
					glm::vec3 monsterangle;
					if (dist <= 6 && cosine < 0.8)
					{
						monsterangle = glm::normalize(p - location1);
					}
					else
					{
						monsterangle = f * glm::vec3(-1.0);
					}

					float cosine = glm::dot(monsterangle, glm::vec3(0, 0, 1));
					float angle = acos(cosine);
					monsterpos1 = glm::mat4(1.0f);
					monsterpos1 = glm::translate(monsterpos1, location1);
					if (f.x < 0)
					{
						monsterpos1 = glm::rotate(monsterpos1, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
					}
					else
					{
						monsterpos1 = glm::rotate(monsterpos1, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
					}
				}
				
				
			}
		}
		if (secondmonster)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(location2 - p);

			glm::vec3 f = mycamera.Front;
			f.y = 0;
			f = glm::normalize(f);

			float cosine = glm::dot(location2 - p, f) / dist;
			if (dist > 6 || dist <= 6 && cosine < 0.8 || crosswall(p, location2))
			{
				glm::vec3 res = getwallline(location2, location2 + glm::normalize(p - location2) * glm::vec3(0.05));

				if (res != glm::vec3(0.0f))
				{
					location2 = location2 + glm::normalize(res) * glm::vec3(0.05);
					float cosine = glm::dot(glm::normalize(res), glm::vec3(0, 0, 1));
					float angle = acos(cosine);
					monsterpos2 = glm::mat4(1.0f);
					monsterpos2 = glm::translate(monsterpos2, location2);
					if (f.x < 0)
					{
						monsterpos2 = glm::rotate(monsterpos2, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
					}
					else
					{
						monsterpos2 = glm::rotate(monsterpos2, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
					}
				}
				else
				{
					location2 = location2 + glm::normalize(p - location2) * glm::vec3(0.05);
					glm::vec3 monsterangle;
					if (dist <= 6 && cosine < 0.8)
					{
						monsterangle = glm::normalize(p - location2);
					}
					else
					{
						monsterangle = f * glm::vec3(-1.0);
					}

					float cosine = glm::dot(monsterangle, glm::vec3(0, 0, 1));
					float angle = acos(cosine);
					monsterpos2 = glm::mat4(1.0f);
					monsterpos2 = glm::translate(monsterpos2, location2);
					if (f.x < 0)
					{
						monsterpos2 = glm::rotate(monsterpos2, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
					}
					else
					{
						monsterpos2 = glm::rotate(monsterpos2, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
					}
				}

			}
		}
		if (thirdmonster)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(location3 - p);

			glm::vec3 f = mycamera.Front;
			f.y = 0;
			f = glm::normalize(f);

			float cosine = glm::dot(location3 - p, f) / dist;
			if (dist > 6 || dist <= 6 && cosine < 0.8 || crosswall(p, location3))
			{
				glm::vec3 res = getwallline(location3, location3 + glm::normalize(p - location3) * glm::vec3(0.05));

				if (res != glm::vec3(0.0f))
				{
					location3 = location3 + glm::normalize(res) * glm::vec3(0.05);
					float cosine = glm::dot(glm::normalize(res), glm::vec3(0, 0, 1));
					float angle = acos(cosine);
					monsterpos3 = glm::mat4(1.0f);
					monsterpos3 = glm::translate(monsterpos3, location3);
					if (f.x < 0)
					{
						monsterpos3 = glm::rotate(monsterpos3, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
					}
					else
					{
						monsterpos3 = glm::rotate(monsterpos3, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
					}
				}
				else
				{
					location3 = location3 + glm::normalize(p - location3) * glm::vec3(0.05);
					glm::vec3 monsterangle;
					if (dist <= 6 && cosine < 0.8)
					{
						monsterangle = glm::normalize(p - location3);
					}
					else
					{
						monsterangle = f * glm::vec3(-1.0);
					}

					float cosine = glm::dot(monsterangle, glm::vec3(0, 0, 1));
					float angle = acos(cosine);
					monsterpos3 = glm::mat4(1.0f);
					monsterpos3 = glm::translate(monsterpos3, location3);
					if (f.x < 0)
					{
						monsterpos3 = glm::rotate(monsterpos3, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
					}
					else
					{
						monsterpos3 = glm::rotate(monsterpos3, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
					}
				}

			}
		}


		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//monsterpos = glm::rotate(monsterpos, currentFrame, glm::vec3(0, 1, 0));
        

		Camera viewcamera = Camera(glm::vec3(0, 3.0 + 20.0f * sin(currentFrame), 20.0 * cos(currentFrame))
			, glm::vec3(0, -1.0f * sin(currentFrame), -1.0 * cos(currentFrame)),
			glm::normalize(glm::vec3(0.0f, 1.0f * cos(currentFrame), 0.0f))
		);
		//std::cout << "pos:" << mycamera.Pos.x << "," << mycamera.Pos.y << "," << mycamera.Pos.z << "," << std::endl;
		//std::cout << "pos:" << mycamera.Front.x << "," << mycamera.Front.y << "," << mycamera.Front.z << "," << std::endl;
		// input
		// -----
		processInput(window);
		if (!gameover)
		{
			

			// render
			// ------
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// don't forget to enable shader before setting uniforms



			ourShader.use();

			ourShader.setVec3("viewPos", mycamera.Pos);
			ourShader.setVec3("lightPos", lightPos);
			ourShader.setFloat("shininess", 128.0f);

			ourShader.setVec3("spotLight.position", mycamera.Pos);
			ourShader.setVec3("spotLight.direction", mycamera.Front);
			ourShader.setVec3("spotLight.ambient", 0.4f, 0.4f, 0.4f);
			ourShader.setVec3("spotLight.diffuse", 0.7f, 0.7f, 0.7f);
			ourShader.setVec3("spotLight.specular", 0.3f, 0.3f, 0.3f);
			ourShader.setFloat("spotLight.constant", 1.0f);
			ourShader.setFloat("spotLight.linear", 0.09f);
			ourShader.setFloat("spotLight.quadratic", 0.032f);
			ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)1920 / (float)1080, 0.1f, 100.0f);
			glm::mat4 view;
			if (freeview)
			{
				view = mycamera.Getview();
			}
			else if (showview)
			{
				view = viewcamera.Getview();
			}
			else if (controlview)
			{
				view = mycamera.Getview();
			}
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", view);
			// render the loaded model

			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);



			monsterShader.use();


			monsterShader.setVec3("viewPos", mycamera.Pos);
			monsterShader.setVec3("lightPos", lightPos);

			monsterShader.setVec3("spotLight.position", mycamera.Pos);
			monsterShader.setVec3("spotLight.direction", mycamera.Front);
			monsterShader.setVec3("spotLight.ambient", 0.7f, 0.7f, 0.7f);
			monsterShader.setVec3("spotLight.diffuse", 0.7f, 0.7f, 0.7f);
			monsterShader.setVec3("spotLight.specular", 0.5f, 0.5f, 0.5f);
			monsterShader.setFloat("spotLight.constant", 1.0f);
			monsterShader.setFloat("spotLight.linear", 0.09f);
			monsterShader.setFloat("spotLight.quadratic", 0.032f);
			monsterShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			monsterShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(25.0f)));
			monsterShader.setMat4("projection", projection);
			monsterShader.setMat4("view", view);


			monsterShader.setMat4("model", monsterpos1);

			monster1.Draw(monsterShader);


			monsterShader.setMat4("model", monsterpos2);

			monster2.Draw(monsterShader);

			monsterShader.setMat4("model", monsterpos3);

			monster3.Draw(monsterShader);

		}


		if (gameover&&!hold)
		{
			glm::vec3 p = mycamera.Pos;
			p.y = 0;
			float dist = glm::length(headlocation - p);

			glm::vec3 f = mycamera.Front;
			f.y = 0;
			f = glm::normalize(f);

			headlocation = p+glm::vec3(0.5f)*f;
			glm::vec3 monsterangle;


			float cosine = glm::dot(f * glm::vec3(-1.0), glm::vec3(0, 0, 1));
			float angle = acos(cosine);
			headpos = glm::mat4(1.0f);
			headpos = glm::translate(headpos, headlocation);
			if (f.x < 0)
			{
				headpos = glm::rotate(headpos, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, 1, 0));
			}
			else
			{
				headpos = glm::rotate(headpos, glm::radians(angle * 180.0f / (3.1415926f)), glm::vec3(0, -1, 0));
			}
			monsterShader.setMat4("model", headpos);
			head.Draw(monsterShader);
			hold = true;
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;


}
bool crosswall(glm::vec3 p1, glm::vec3 p2)
{
	p1.y = 0;
	p2.y = 0;
	for (auto it = visited.begin(); it != visited.end(); it++)
	{
		glm::vec3 p3 = glm::vec3(it->first.x1, 0, it->first.z1);
		glm::vec3 p4 = glm::vec3(it->first.x2, 0, it->first.z2);

		float x_min1 = min(p1.x, p2.x);
		float x_max1 = max(p1.x, p2.x);

		float x_min2 = min(p3.x, p4.x);
		float x_max2 = max(p3.x, p4.x);

		float z_min1 = min(p1.z, p2.z);
		float z_max1 = max(p1.z, p2.z);

		float z_min2 = min(p3.z, p4.z);
		float z_max2 = max(p3.z, p4.z);

		if (x_max1 >= x_min2 && x_max2 >= x_min1 && z_max1 >= z_min2 && z_max2 >= z_min1)
		{
			float temp1 = glm::cross(p1 - p3, p4 - p3).y * glm::cross(p2 - p3, p4 - p3).y;
			float temp2= glm::cross(p4 - p1, p2 - p1).y * glm::cross(p3 - p1, p2 - p1).y;

			if (temp1 <= 0 && temp2 <= 0)
			{
				return true;
			}
		}
	}
	return false;
}
glm::vec3 getwallline(glm::vec3 p, glm::vec3 p1)
{
	p.y = 0;
	p1.y = 0;
	glm::vec3 res = glm::vec3(0.0f);
	for (auto it = visited.begin(); it != visited.end(); it++)
	{
		bool judge = p.x<max(it->first.x1, it->first.x2) && p.x>  min(it->first.x1, it->first.x2) ||
			p.z<max(it->first.z1, it->first.z2) && p.z>  min(it->first.z1, it->first.z2);
		//std::cout << judge << std::endl;

		if (!judge)
		{
			continue;
		}

		glm::vec3 l1 = p - glm::vec3(it->first.x1, 0, it->first.z1);
		glm::vec3 l2 = glm::vec3(it->first.x2, 0, it->first.z2) - glm::vec3(it->first.x1, 0, it->first.z1);
		glm::vec3 l3 = p1 - glm::vec3(it->first.x1, 0, it->first.z1);
		float d1 = glm::length(l1);
		float d2 = glm::length(l2);
		float d3 = glm::length(l3);



		float cos = glm::dot(l1, l2) / d2 / d1;
		float cos1 = glm::dot(l3, l2) / d2 / d3;

		float sin = sqrt(1 - pow(cos, 2));
		float sin1 = sqrt(1 - pow(cos1, 2));

		float dist = d1 * sin;
		float dist1 = d3 * sin1;
		if (abs(dist) < 1)
		{

			if (abs(dist) > abs(dist1))
			{
				glm::vec3 res1= glm::vec3(it->first.x2, 0, it->first.z2) - glm::vec3(it->first.x1, 0, it->first.z1);
				glm::vec3 res2= glm::vec3(it->first.x1, 0, it->first.z1) -glm::vec3(it->first.x2, 0, it->first.z2);
				glm::vec3 move = p1 - p;
				if (glm::dot(res1, move) < 0)
				{
					res = res2;
				}
				else
				{
					res = res1;
				}
			}

		}
	}
	return res;
}

bool wallcollision(glm::vec3 p,glm::vec3 p1)
{
	p.y = 0;
	p1.y = 0;

	for (auto it = visited.begin(); it != visited.end(); it++)
	{
		bool judge= p.x<max(it->first.x1, it->first.x2) && p.x>  min(it->first.x1, it->first.x2) ||
			p.z<max(it->first.z1, it->first.z2) && p.z>  min(it->first.z1, it->first.z2);
		//std::cout << judge << std::endl;

		if (!judge)
		{
			continue;
		}

		glm::vec3 l1 = p - glm::vec3(it->first.x1, 0, it->first.z1);
		glm::vec3 l2 = glm::vec3(it->first.x2, 0, it->first.z2) - glm::vec3(it->first.x1, 0, it->first.z1);
		glm::vec3 l3=  p1 - glm::vec3(it->first.x1, 0, it->first.z1);
		float d1 = glm::length(l1);
		float d2 = glm::length(l2);
		float d3 = glm::length(l3);



		float cos = glm::dot(l1, l2) / d2/d1;
		float cos1 = glm::dot(l3, l2) / d2/d3;

		float sin = sqrt(1 - pow(cos, 2));
		float sin1 = sqrt(1 - pow(cos1, 2));

		float dist = d1 * sin;
		float dist1 = d3 * sin1;
		if (abs(dist) < 0.2)
		{

			if (abs(dist) > abs(dist1))
			{
				return true;
			}
			
		}
	}
	return false;
}



void mouse_presscallback(GLFWwindow* window, int button, int action, int mods)
{
	if (freeview)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			lightColor += glm::vec3(0.1, 0.1, 0.1);
			if (sundir && sundist < 1.5)
			{
				std::cout << "点了太阳" << std::endl;
			}
			if (earthdir && earthdist < 1.0)
			{
				std::cout << "点了地球" << std::endl;
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			lightColor -= glm::vec3(0.1, 0.1, 0.1);
		}
	}
	else if (controlview)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			rotateflag = true;
			std::cout << true << std::endl;
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			rotateflag = false;
		}
	}

}

void processInput(GLFWwindow* window)
{
	float cameraspeed = deltaTime * 10.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS&&gameover)
	{
		
		firstmonster = true;
		secondmonster = false;
		thirdmonster = false;
		hold = false;
		m2boot = false;
		rotatecount2 = 0;
		m3boot = false;
		rotatecount3 = 0;
		firstMouse = true;

		mycamera.Pos = glm::vec3(-4.85027, 0, -13.7127);
		mycamera.Front = glm::vec3(-0.99824, 0.0593061, -1.13284e-06);
		mycamera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
		mycamera.U = glm::vec3(0.0f, 1.0f, 0.0f);
		mycamera.Yaw = 180.0f;
		mycamera.Pitch = 0.0f;

		headpos = glm::mat4(1.0f);
		headpos = glm::translate(headpos, glm::vec3(-7.0f, 0.0f, -7.0f));
		headpos = glm::rotate(headpos, glm::radians(0.0f), glm::vec3(0, 1, 0));

		monsterpos2 = glm::mat4(1.0f);
		monsterpos2 = glm::translate(monsterpos2, glm::vec3(-6.54106, 0, -6.38201));
		monsterpos2 = glm::rotate(monsterpos2, glm::radians(180.0f), glm::vec3(0, 1, 0));

		monsterpos1 = glm::mat4(1.0f);
		monsterpos1 = glm::translate(monsterpos1, glm::vec3(-8.54481, 0, -13.6378));
		monsterpos1 = glm::rotate(monsterpos1, glm::radians(90.0f), glm::vec3(0, 1, 0));

		monsterpos3 = glm::mat4(1.0f);
		monsterpos3 = glm::translate(monsterpos3, glm::vec3(6.17501, 0, 12.6247));
		monsterpos3 = glm::rotate(monsterpos3, glm::radians(90.0f), glm::vec3(0, 1, 0));

		gameover = false;
	}


    
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS&&!gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos += cameraspeed * front;
			pos -= cameraspeed * glm::normalize(glm::cross(front, mycamera.Up));
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}
		else if (controlview)
		{
			transvector += cameraspeed * mycamera.U;
			transvector -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.U));
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos += cameraspeed * front;
			pos += cameraspeed * glm::normalize(glm::cross(front, mycamera.Up));
			if (wallcollision(mycamera.Pos,pos) == false)
			{
				mycamera.Pos = pos;
			}
		}
		else if (controlview)
		{
			transvector += cameraspeed * mycamera.U;
			transvector += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos -= cameraspeed * front;
			pos -= cameraspeed * glm::normalize(glm::cross(front, mycamera.Up));
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}
		else if (controlview)
		{
			transvector -= cameraspeed * mycamera.U;
			transvector -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos -= cameraspeed * front;
			pos += cameraspeed * glm::normalize(glm::cross(front, mycamera.Up));
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}
		else if (controlview)
		{
			transvector -= cameraspeed * mycamera.U;
			transvector += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos += cameraspeed * front;
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}


		else if (controlview)
		{
			transvector += cameraspeed * mycamera.U;
		}
	}

	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos -= cameraspeed * front;
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}

			
		else if (controlview)
		{
			transvector -= cameraspeed * mycamera.U;
		}
	}

	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos -= cameraspeed * glm::normalize(glm::cross(front, mycamera.Up));
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}

			
		else if (controlview)
		{
			transvector -= cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
	}

	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !gameover)
	{
		if (freeview)
		{
			glm::vec3 front = mycamera.Front;
			front.y = 0;
			front = glm::normalize(front);
			glm::vec3 pos = mycamera.Pos;
			pos += cameraspeed * glm::normalize(glm::cross(front, mycamera.Up));
			if (wallcollision(mycamera.Pos, pos) == false)
			{
				mycamera.Pos = pos;
			}
		}
		else if (controlview)
		{
			transvector += cameraspeed * glm::normalize(glm::cross(mycamera.Front, mycamera.Up));
		}
	}


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{


	if (firstMouse)
	{
		//std::cout << xpos << "," << ypos << endl;
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}



	if (freeview)
	{


		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.09;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		mycamera.Yaw += xoffset;
		mycamera.Pitch += yoffset;

		if (mycamera.Pitch > 89.0f)
			mycamera.Pitch = 89.0f;
		if (mycamera.Pitch < -89.0f)
			mycamera.Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(mycamera.Yaw)) * cos(glm::radians(mycamera.Pitch));
		front.y = sin(glm::radians(mycamera.Pitch));
		front.z = sin(glm::radians(mycamera.Yaw)) * cos(glm::radians(mycamera.Pitch));

		glm::vec3 up;

		up.x = sin(glm::radians(-1.0 * mycamera.Pitch)) * cos(glm::radians(mycamera.Yaw));
		up.y = cos(glm::radians(-1.0 * mycamera.Pitch));
		up.z = sin(glm::radians(-1.0 * mycamera.Pitch)) * sin(glm::radians(mycamera.Yaw));

		//std::cout << front.x << "," << front.y << "," << front.z << std::endl;
		//std::cout << up.x << "," << up.y << "," << up.z << std::endl;
		mycamera.Front = glm::normalize(front);
		mycamera.U = glm::normalize(up);
	}
	else if (controlview)
	{
		if (rotateflag)
		{

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			if (xoffset == 0 && yoffset == 0)
			{
				return;
			}
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.05;
			xoffset *= sensitivity;
			yoffset *= sensitivity;


			rotateaxis = mycamera.U * xoffset + glm::cross(mycamera.U, mycamera.Front) * yoffset;
			rotateaxis = glm::normalize(rotateaxis);
			if (xoffset < 0)
			{
				objradius = sqrt(pow(xoffset, 2) + pow(yoffset, 2));
			}
			else
			{
				objradius = sqrt(pow(xoffset, 2) + pow(yoffset, 2));
			}



		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (freeview)
	{
		fov -= (float)yoffset;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
	}
	else if (controlview)
	{
		s -= (float)yoffset / 45.0;
		if (s < 0.1f)
			s = 0.1f;
		if (s > 10.0f)
			s = 10.0f;
	}

}