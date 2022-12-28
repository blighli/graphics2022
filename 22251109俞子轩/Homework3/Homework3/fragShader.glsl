#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;

in vec2 tc;
out vec4 fragColor;
struct PositionalLight
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
};
uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

layout (binding=0) uniform sampler2D s;

void main(void)
{
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	
	float cosTheta = dot(L,N);
	vec3 H = normalize(varyingHalfVector);
	float cosPhi = dot(H,N);

	vec4 textcolor = texture(s,tc);
	fragColor = textcolor * (globalAmbient+light.ambient+light.diffuse*max(cosTheta,0.0))+light.specular*pow(max(cosPhi,0.0),3.0);
}
