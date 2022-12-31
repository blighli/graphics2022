#version 330 core
layout (location = 0) in mat4 instanceMatrix;
layout (location = 4) in vec3 aPos;
layout (location = 5) in vec3 aNormal;
layout (location = 6) in vec2 aTexCoords;
layout (location = 7) in ivec4 BoneIDs;
layout (location = 8) in vec4 Weights;

layout (std140) uniform baseMatrices
{
    mat4 projection;
    mat4 view;
    mat4 lightSpace;

    vec3 viewPos;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 model;
uniform int useInstance;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];
uniform int hasBone;

out VS_OUT{
    vec3 viewPos;
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;

    // Lighting
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} vs_out;


void main()
{
    vec4 pos;
    vec3 normal;
    if(hasBone == 0)
    {
        pos = vec4(aPos, 1.0f);
        normal = aNormal;
    }
    else
    {
        mat4 bone_transform = bones[BoneIDs[0]] * Weights[0];
	    bone_transform += bones[BoneIDs[1]] * Weights[1];
	    bone_transform += bones[BoneIDs[2]] * Weights[2];
	    bone_transform += bones[BoneIDs[3]] * Weights[3];

        pos = bone_transform * vec4(aPos, 1.0f);
	    normal = vec3(bone_transform * vec4(aNormal, 0.0f));
    }

    mat4 modelMat;
    if(useInstance == 0) modelMat = model;
    else modelMat = instanceMatrix;

    gl_Position = projection * view * modelMat * pos;

    vs_out.viewPos = viewPos;
    vs_out.fragPos = vec3(modelMat * pos);

    vs_out.normal = mat3(transpose(inverse(modelMat))) * normal;
    vs_out.texCoords = aTexCoords;
    vs_out.direction = direction;
    vs_out.ambient = ambient;
    vs_out.diffuse = diffuse;
    vs_out.specular = specular;
}