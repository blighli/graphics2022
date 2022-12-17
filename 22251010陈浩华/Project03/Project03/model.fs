#version 330 core
out vec4 FragColor;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointLight[2];
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcPointLight(pointLight[0], norm, FragPos, viewDir);
    result += CalcPointLight(pointLight[1], norm, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // ambient
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords)) * 1.5;
    // diffuse
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * vec3(texture(texture_diffuse1, TexCoords)) * diff * attenuation;
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specular = light.specular * spec * attenuation;

    return ambient + diffuse + specular;
}