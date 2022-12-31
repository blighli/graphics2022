#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;

void main()
{
    vec4 objectColor = texture(texture1, TexCoord);
    vec3 lightDir = normalize(-FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    FragColor = (0.1 + diff) * objectColor;
}