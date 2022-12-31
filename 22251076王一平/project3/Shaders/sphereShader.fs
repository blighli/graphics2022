#version 460 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texTure1;

void main()
{
   FragColor = texture(texTure1, TexCoord);
   //FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
}