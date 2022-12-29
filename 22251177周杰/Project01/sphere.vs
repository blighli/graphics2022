#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    //ourColor = vec3(126.0f / 256.0f, 12.0f / 256.0f, 110.0f / 256.0f);
    TexCoord = aTexCoord;
}