#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;

// ACES tone map
// see: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
vec3 toneMapACES(vec3 color)
{
  const float A = 2.51;
  const float B = 0.03;
  const float C = 2.43;
  const float D = 0.59;
  const float E = 0.14;
  return clamp((color * (A * color + B)) / (color * (C * color + D) + E), 0.0, 1.0);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord;
    vec4 color = texture(screenTexture, uv);
    float strength = 16.0;

    float x = (uv.x + 4.0 ) * (uv.y + 4.0 ) * (1.0 * 10.0);
	vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * strength;
    
    grain = 1.0 - grain;
	fragColor = color * grain;

    //color = (abs(uv.x - 0.5) < 0.002) ? vec4(0.0) : color;
    //fragColor = (uv.x > 0.5) ? color * (1- grain) : color + grain;
}


void main(){
    mainImage(FragColor, TexCoord);
    // HDR tonemapping
    //FragColor = vec4(FragColor.rgb / (FragColor.rgb + vec3(1.0)), 1.0f);
}
