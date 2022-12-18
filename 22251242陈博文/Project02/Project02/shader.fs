#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
struct Light {
    vec3 position;
    //vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

     float constant;
    float linear;
    float quadratic;
};

struct Material {

    sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Light light;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 viewPos;
uniform vec3 lightPos; 
uniform vec3 lightColor;  
uniform Material material;  
//in vec4 color;
void main()
{
    float distance=length(light.position-FragPos);
    // float attenuaion=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    float attenuaion = 1.0; 
    vec3 ambient  = light.ambient *vec3(texture(material.diffuse,TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position-FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff* vec3(texture(material.diffuse,TexCoords));

      // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec*material.specular;  

  

    ambient*=attenuaion;
    
    specular*=attenuaion;
    
    diffuse*=attenuaion;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);

    //FragColor=texture(Texture, TexCoord);
}
