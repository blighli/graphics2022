#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;

void main()
{   vec3 light_dir = vec3(-0.2, -1.0, -0.3);
    vec3 lightDir = normalize(-light_dir);
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = 0.1 * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse = 0.5 * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = 1.0 * spec * vec3(texture(texture_specular1, TexCoords));

    FragColor = vec4(ambient+diffuse+specular, 1.0);
}