#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
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
#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform Material material;
uniform PointLight pointLight;
uniform vec3 viewPos;
uniform bool isSun;

vec3 CalPointLight(PointLight light, vec3 fragpos, vec3 norm, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

	vec3 lightDir = normalize(light.position - fragpos);
	float alpha = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * alpha * texture(material.diffuse, TexCoord).rgb;

	vec3 reflDir = normalize(-lightDir - 2*(dot(-lightDir, norm))*norm);
	float beta = pow(max(dot(reflDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * beta;// * texture(material.specular, TexCoord).rgb;
	
	float dis = length(light.position - fragpos);
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));

	return (ambient + diffuse) * attenuation;
}

void main()
{
	if(isSun){
		FragColor = vec4(texture(material.diffuse, TexCoord).rgb, 1.0f);
	}
	else{
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 result = CalPointLight(pointLight, FragPos, norm, viewDir);
		FragColor = vec4(result, 1.0f);
	}
}
