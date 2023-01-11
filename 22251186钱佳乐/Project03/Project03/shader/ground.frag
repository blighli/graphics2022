#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float dist = clamp((currentDepth - closestDepth) / currentDepth, 0.0f, 1.0f);
    int samples = (dist < 0.8)? ((dist < 0.3) ? 1 : 2) : 3;
    //samples = 3;
    for(int x = -samples; x <= samples; ++x)
    {
        for(int y = -samples; y <= samples; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    float axisPer = 2.0f * samples + 1.0f;
    shadow /= axisPer*axisPer;

    return shadow;
}

void main(){
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 color = vec3(0.14) * shadow + (1 - shadow) * vec3(0.21);
    FragColor = vec4(color, 1.0f);
}