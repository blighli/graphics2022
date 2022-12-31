#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Sphere.h"



const unsigned int width = 800;
const unsigned int height = 800;

glm::mat4 buildScale(float x, float y, float z)
{
	glm::mat4 scale = glm::mat4(x, 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0,
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return scale;
}


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "HW_2", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);

	Sphere sphere;

	//Ì«Ñô
	Shader sunShader("sun.vert", "sun.frag");

	VAO VAO_sun;
	VAO_sun.Bind();
	VBO VBO_sun(sphere.verts, sizeof(sphere.verts));
	EBO EBO_sun(sphere.indices, sizeof(sphere.indices));
	VAO_sun.LinkAttrib(VBO_sun, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO_sun.LinkAttrib(VBO_sun, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_sun.LinkAttrib(VBO_sun, 3, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	VAO_sun.Unbind();
	VBO_sun.Unbind();
	EBO_sun.Unbind();


	//µØÇò
	Shader earthShader("plant.vert", "plant.frag");

	VAO VAO_earth;
	VAO_earth.Bind();
	VBO VBO_earth(sphere.verts, sizeof(sphere.verts));
	EBO EBO_earth(sphere.indices, sizeof(sphere.indices));
	VAO_earth.LinkAttrib(VBO_earth, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO_earth.LinkAttrib(VBO_earth, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_earth.LinkAttrib(VBO_earth, 3, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	VAO_earth.Unbind();
	VBO_earth.Unbind();
	EBO_earth.Unbind();


	//ÔÂÁÁ
	Shader moonShader("plant.vert", "plant.frag");

	VAO VAO_moon;
	VAO_moon.Bind();
	VBO VBO_moon(sphere.verts, sizeof(sphere.verts));
	EBO EBO_moon(sphere.indices, sizeof(sphere.indices));
	VAO_moon.LinkAttrib(VBO_moon, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO_moon.LinkAttrib(VBO_moon, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO_moon.LinkAttrib(VBO_moon, 3, 3, GL_FLOAT, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	VAO_moon.Unbind();
	VBO_moon.Unbind();
	EBO_moon.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 sunModel = buildScale(0.5f, 0.5f, 0.5f);
	sunModel = glm::translate(sunModel, sunPos);


	sunShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(sunShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));

	

	// ÎÆÀíÉèÖÃ
	std::string sunPath = "sun.png";
	Texture sunTex(sunPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	sunTex.texUnit(sunShader, "tex_sun", 0);

	std::string earthPath = "earth.png";
	Texture earthTex(earthPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	earthTex.texUnit(earthShader, "tex_earth", 0);

	std::string moonPath = "moon.png";
	Texture moonTex(moonPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	moonTex.texUnit(moonShader, "tex_moon", 0);


	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 10.0f));

	unsigned long long int frame=0;
	while (!glfwWindowShouldClose(window))
	{
		frame++;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);


		//Ì«Ñô
		sunShader.Activate();
		camera.Matrix(sunShader, "camMatrix");
		sunTex.Bind();
		VAO_sun.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(sphere.indices) / sizeof(int), GL_UNSIGNED_INT, 0);



		//µØÇò
		earthShader.Activate();


		glm::vec3 earthPos = glm::vec3(	sin(frame * PI * 0.0001) * 8, 
										0.0,  
										cos(frame * PI * 0.0001) * 8);
		glm::mat4 earthModel = buildScale(0.3f, 0.3f, 0.3f);
		earthModel = glm::translate(earthModel, earthPos);

		glUniformMatrix4fv(glGetUniformLocation(earthShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(earthModel));
		glUniform4f(glGetUniformLocation(earthShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(earthShader.ID, "lightPos"), sunPos.x, sunPos.y, sunPos.z);

		glUniform3f(glGetUniformLocation(earthShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(earthShader, "camMatrix");
		earthTex.Bind();
		VAO_earth.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(sphere.indices) / sizeof(int), GL_UNSIGNED_INT, 0);


		//ÔÂÁÁ
		moonShader.Activate();

		glm::vec3 moonPos = glm::vec3(earthPos.x*3+ sin(frame * PI * 0.0005) * 5, 0.0f, earthPos.z*3+ cos(frame * PI * 0.0005) * 5);
		glm::mat4 moonModel = buildScale(0.1f, 0.1f, 0.1f);
		moonModel = glm::translate(moonModel, moonPos);

		moonShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(moonShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
		glUniform4f(glGetUniformLocation(moonShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(moonShader.ID, "lightPos"), sunPos.x, sunPos.y, sunPos.z);


		glUniform3f(glGetUniformLocation(moonShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(moonShader, "camMatrix");
		moonTex.Bind();
		VAO_moon.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(sphere.indices) / sizeof(int), GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	VAO_sun.Delete();
	VBO_sun.Delete();
	EBO_sun.Delete();
	VAO_earth.Delete();
	VBO_earth.Delete();
	EBO_earth.Delete();
	VAO_moon.Delete();
	VBO_moon.Delete();
	EBO_moon.Delete();

	sunTex.Delete();
	earthTex.Delete();
	moonTex.Delete();

	sunShader.Delete();
	earthShader.Delete();
	moonShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}