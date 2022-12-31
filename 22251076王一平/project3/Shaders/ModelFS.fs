#version 330 core
in vec4 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

struct DirLight {
	vec3 direction;
	vec3 color;
	float Ka;
	float Kd;
	float Ks;
	float shininess;
};

uniform DirLight SunLight;
uniform DirLight EastLight;
uniform DirLight WestLight;
uniform DirLight SouthLight;
uniform DirLight NorthLight;

vec4 CalcDirLight(DirLight light, vec4 texColor) 
{
	vec3 amb, diff, spec;
	vec3 l = normalize(-light.direction);
	vec3 v = normalize(-Position.xyz);
	vec3 r = reflect(-l, Normal);

	amb = light.color * light.Ka;
	float NDotl = max(dot(Normal, l), 0.0);
	diff = light.color * light.Kd * NDotl;
	spec = vec3(0.0);
	if (NDotl > 0.0)
		spec = light.color * light.Ks * pow(max(dot(v, r), 0.0), light.shininess);

	return ((vec4(diff + amb, 1.0) * texColor) + vec4(spec, 1.0));
}

void main(void)
{
	vec4 texColor = texture(texture_diffuse1, TexCoords);
	vec4 res = vec4(0.0, 0.0, 0.0, 0.0);

	res += CalcDirLight(SunLight, texColor);
	res += CalcDirLight(EastLight, texColor);
	res += CalcDirLight(WestLight, texColor);
	res += CalcDirLight(SouthLight, texColor);
	res += CalcDirLight(NorthLight, texColor);

	FragColor = res;
}
