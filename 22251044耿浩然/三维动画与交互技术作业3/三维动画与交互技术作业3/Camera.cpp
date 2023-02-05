#include"Camera.h"

#define M_PI 3.1415926


Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}



void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		mode = FREEDOM_CAMERA;
		Position = glm::vec3(0.0f, 0.0f, 2.0f);
		Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		mode = MODEL_CAMERA;
		Position = glm::vec3(0.0f, 0.0f, 2.0f);
		Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		degreeXY = 0;
		degreeZ = 90;
	}

	if (mode == FREEDOM_CAMERA)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Position += speed * Orientation;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Position += speed * -glm::normalize(glm::cross(Orientation, Up));

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position += speed * -Orientation;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Position += speed * glm::normalize(glm::cross(Orientation, Up));

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			Position += speed * Up;

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			Position += speed * -Up;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			speed = 0.01f;

		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			speed = 0.001f;



		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (firstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				firstClick = false;
			}

			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				Orientation = newOrientation;
			}

			Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
		}
	}
	else if (mode == MODEL_CAMERA)
	{
		glm::vec3 Dir = glm::vec3(0.0f, 0.0f, 0.0f) - Position;
		float length = std::sqrt(Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			length -= speed;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			length += speed;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			speed = 0.01f;

		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
			speed = 0.001f;

		if (length < 0.1)
			length = 0.1;


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (firstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				firstClick = false;
			}

			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;


			degreeXY += rotY;

			degreeZ += rotX;
			if (degreeZ > 179.9)
				degreeZ = 179.9;
			else if (degreeZ < 0.1)
				degreeZ = 0.1;

			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstClick = true;
		}

		float px = length * std::sin(degreeZ / 180 * M_PI) * std::sin(degreeXY / 360.0 * M_PI);
		float py = length * std::cos(degreeZ / 180 * M_PI);
		float pz = length * std::sin(degreeZ / 180 * M_PI) * std::cos(degreeXY / 360.0 * M_PI);

		glm::vec3 newPosition = glm::vec3(px, py, pz);
		Position = newPosition;

		Orientation = glm::vec3(0,0,0) - newPosition;
		Orientation.x /= length;
		Orientation.y /= length;
		Orientation.z /= length;


	}
}