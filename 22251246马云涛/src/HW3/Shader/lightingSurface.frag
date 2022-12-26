#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 viewPos;
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;

    // Lighting
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} fs_in;

uniform sampler2D texture_diffuse;
uniform int useColor;
uniform vec3 diffuseColor;

vec3 CalcDirLight(vec3 dir,vec3 a,vec3 d,vec3 s, vec3 normal, vec3 viewDir,vec3 color);

void main()
{
    // 属性
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.fragPos);
    vec3 color;
    if(useColor == 1)
    {
        color = diffuseColor;
    }
    else
    {
        color = vec3(texture(texture_diffuse, fs_in.texCoords).xyz);
    }

    vec3 result = CalcDirLight(fs_in.direction,fs_in.ambient,fs_in.diffuse,fs_in.specular, norm, viewDir,color);
    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(vec3 dir,vec3 a,vec3 d,vec3 s, vec3 normal, vec3 viewDir,vec3 color)
{
    vec3 lightDir = normalize(-dir);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(dir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);

    vec3 ambient  = a;
    vec3 diffuse  = d * diff;
    vec3 specular = s * spec;

    return (ambient + (diffuse + specular)) * color;;
}
