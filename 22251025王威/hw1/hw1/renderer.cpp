#include "renderer.h"
#include <glm/gtc/type_ptr.hpp>

Renderer2D::Renderer2D() {
	// bind texture
	square_texture_ = loadTexture("./image/image1.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, square_texture_);

	// square
	square_shader_ = loadShader("./res/rectangular.vert", "./res/rectangular.frag");
	glUseProgram(square_shader_);

	glGenVertexArrays(1, &square_buffers_.VAO);
	glBindVertexArray(square_buffers_.VAO);

	glGenBuffers(1, &square_buffers_.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, square_buffers_.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &square_buffers_.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_buffers_.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

}

void Renderer2D::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderSquare();
}

void Renderer2D::renderSquare() {
	glUseProgram(square_shader_);
	glUniform1i(glGetUniformLocation(square_shader_, "texture1"), 0);
	glBindVertexArray(square_buffers_.VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Renderer3D::Renderer3D(int width, int height) :
	width_(width), height_(height)
{
	aspect_ratio_ = static_cast<float>(width_) / height_;
	//camera.Position = Position;
	//camera.Front = Front;
	//camera.Up = Up;
	//view_ = origin_camera_;
	projection_ = glm::perspective(glm::radians(fov_), static_cast<float>(width_) / height_, 0.1f, 100.0f);
	// border
	border_shader_ = loadShader("./res/border.vert", "./res/border.frag");
	glUseProgram(border_shader_);
	glUniformMatrix4fv(glGetUniformLocation(border_shader_, "projection"), 1, GL_FALSE, glm::value_ptr(projection_));

	glGenVertexArrays(1, &border_buffers_.VAO);
	glBindVertexArray(border_buffers_.VAO);

	glGenBuffers(1, &border_buffers_.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, border_buffers_.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(border_vertices_), border_vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &border_buffers_.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_buffers_.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border_indices_), border_indices_, GL_STATIC_DRAW);
}

void Renderer3D::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	view_ = camera.GetViewMatrix();
	renderBorder();
	renderParticles();
}

void Renderer3D::renderBorder() {
	glUseProgram(border_shader_);
	glUniformMatrix4fv(glGetUniformLocation(border_shader_, "view"), 1, GL_FALSE, glm::value_ptr(view_));
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(glGetUniformLocation(border_shader_, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}

void Renderer3D::renderParticles() {

}
