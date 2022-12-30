#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex_sun;

void main()
{
	FragColor = texture(tex_sun, texCoord);
}