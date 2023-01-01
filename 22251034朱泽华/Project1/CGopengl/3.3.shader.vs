#version 330 core
layout(location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0
layout(location = 1) in vec2 aTexCoord; // 颜色变量的属性位置值为 1
//layout(location = 2) in vec2 aTexCoord; // 纹理坐标属性位置值事 2

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   //vertexColor = vec4(aColor, 1.0);
   TexCoord = aTexCoord;
};