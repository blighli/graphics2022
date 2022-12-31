
#ifndef RENDERER_H
#define RENDERER_H
#include "texture.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

struct GLBuffers {
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};
class Renderer2D {
public:
	Renderer2D();
	~Renderer2D() {}
	void render();

private:
	const GLfloat vertices_[32] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
	};

	const GLuint indices_[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLuint square_shader_;
	GLBuffers square_buffers_;
	GLuint square_texture_;

	void renderSquare();

};


class Renderer3D {
public:
	Renderer3D(int width, int height);
	~Renderer3D() {}
	void render();
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 Position = glm::vec3(5.5f, 1.2f, 5.5f);
	glm::vec3 Front = glm::vec3(-5.5f, 0.0f, -5.5f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);


	const GLfloat border_vertices_[48] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};

	const GLuint border_indices_[24] = {
		0, 1,
		0, 3,
		0, 4,
		2, 3,
		2, 1,
		2, 6,
		3, 7,
		1, 5,
		4, 7,
		4, 5,
		6, 7,
		6, 5
	};

	glm::mat4 origin_camera_ = glm::lookAt(Position,  // camera position
		Position + Front,  // target position
		Up   // up vector
	);

	//glm::mat4 up_camera_ = glm::lookAt(glm::vec3(0.0f, 6.8f, 0.0f),  // camera position
	//	glm::vec3(0.0f, 1.2f, 0.0f),  // target position
	//	glm::vec3(0.0f, 0.0f, -1.0f)   // up vector
	//);

	//glm::mat4 front_camera_ = glm::lookAt(glm::vec3(0.0f, 1.2f, 7.0f),  // camera position
	//	glm::vec3(0.0f, 1.2f, 0.0f),  // target position
	//	glm::vec3(0.0f, 1.0f, 0.0f)   // up vector
	//);

	//glm::mat4 side_camera_ = glm::lookAt(glm::vec3(7.0f, 1.2f, 0.0f),  // camera position
	//	glm::vec3(0.0f, 1.2f, 0.0f),  // target position
	//	glm::vec3(0.0f, 1.0f, 0.0f)   // up vector
	//);

	// window size
	int width_;
	int height_;
	float aspect_ratio_;

	glm::mat4 view_;
	glm::mat4 projection_;
	float fov_ = 45.0f;

	// snow point size
	GLfloat radius_ = 0.015f;

	GLuint water_shader_;
	GLuint border_shader_;
	GLBuffers water_buffers_;
	GLBuffers border_buffers_;

	void renderBorder();
	void renderParticles();
};
#endif