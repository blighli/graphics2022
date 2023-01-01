#version 330 core
out vec4 FragColor;
in vec4 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ )
in vec2 TexCoord;
uniform vec4 ourColor; // ��OpenGL����������趨�������

uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3);
};
