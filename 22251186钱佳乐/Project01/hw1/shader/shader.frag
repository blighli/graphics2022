#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 4

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 fragpos, vec3 normal, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 fragpos, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		result += CalPointLight(pointLights[i], FragPos, norm, viewDir);
	}
	result += CalSpotLight(spotLight, FragPos, norm, viewDir);

	FragColor = vec4(result, 1.0f);
}

vec3 CalDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	
	float alpha = max(dot(norm, -light.direction), 0.0f);
	vec3 diffuse = light.diffuse * alpha * texture(material.diffuse, TexCoord).rgb;
	
	vec3 reflDir = light.direction - 2*(dot(light.direction, norm))*norm;
	float beta = pow(max(dot(viewDir, reflDir), 0.0f), material.shininess);
	vec3 specular = light.specular * beta * texture(material.specular, TexCoord).rgb;
	
	return ambient + diffuse + specular;
}

vec3 CalPointLight(PointLight light, vec3 fragpos, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	vec3 lightDir = normalize(light.position - fragpos);
	float alpha = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * alpha * texture(material.diffuse, TexCoord).rgb;

	vec3 reflDir = normalize(-lightDir - 2*(dot(-lightDir, norm))*norm);
	float beta = pow(max(dot(reflDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * beta * texture(material.specular, TexCoord).rgb;
	
	float dis = length(light.position - fragpos);
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalSpotLight(SpotLight light, vec3 fragpos, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	
	vec3 lightDir = normalize(light.position - FragPos);
	float alpha = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * alpha * vec3(texture(material.diffuse, TexCoord));

	vec3 reflDir = normalize(-lightDir - 2*(dot(norm, -lightDir))*norm); //reflect(-lightDir, norm)
	float beta = pow(max(dot(viewDir, reflDir), 0.0f), material.shininess);
	vec3 specular = light.specular * beta * vec3(texture(material.specular, TexCoord));

	float theta = dot(-light.direction, lightDir);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	float dis = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));

	return (ambient + diffuse + specular) * attenuation;
}

    //spotLight
    // {
    //     vec3 pos = vec3(-0.25, 0, 1);
    //     vec3 dir = vec3(0, 0, -1);
    //     float cutOff = 0.99978;//cos(12.5);
    //     float outerCutOff = 0.9953;//cos(17.5);
    //     vec3 L = normalize(pos - WorldPos);
    //     float epsilon = cutOff - outerCutOff;
    //     float intensity = clamp((dot(L, -dir) - outerCutOff) / epsilon, 0.0f, 1.0f);
    //     float dist = length(pos - WorldPos);
    //     float attenuation = 1/(dist*dist);
    //     vec3 radiance = lightColor/10 * attenuation * intensity;
    // }