#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
	texCoord = aTex;
}