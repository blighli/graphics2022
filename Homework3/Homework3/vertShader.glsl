#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
out vec2 tc;
out vec3 varyingNormal;
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;

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
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	tc = tex_coord;
}