#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
// Gets the color of the light from the main function
uniform vec4 lightColor_1;
// Gets the position of the light from the main function
uniform vec3 lightPos_1;
// Gets the color of the light from the main function
uniform vec4 lightColor_2;
// Gets the position of the light from the main function
uniform vec3 lightPos_2;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection_1 = normalize(lightPos_1 - crntPos);
	vec3 lightDirection_2 = normalize(lightPos_2 - crntPos);
	float diffuse_1 = max(dot(normal, lightDirection_1), 0.0f);
	float diffuse_2 = max(dot(normal, lightDirection_2), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection_1 = reflect(-lightDirection_1, normal);
	vec3 reflectionDirection_2 = reflect(-lightDirection_2, normal);
	float specAmount_1 = pow(max(dot(viewDirection, reflectionDirection_1), 0.0f), 8);
	float specAmount_2 = pow(max(dot(viewDirection, reflectionDirection_2), 0.0f), 8);
	float specular_1 = specAmount_1 * specularLight;
	float specular_2 = specAmount_2 * specularLight;

	//merge color
	vec4 color_1 = lightColor_1 * (diffuse_1 + ambient + specular_1);
	vec4 color_2 = lightColor_2 * (diffuse_2 + ambient + specular_2);
	vec4 color = vec4(min(color_1.x + color_2.x, 1.0), min(color_1.y + color_2.y, 1.0), min(color_1.z + color_2.z, 1.0), 1.0);

	// outputs final color
	//FragColor = texture(tex0, texCoord) * lightColor_1 * (diffuse_1 + ambient + specular_1);
	FragColor = texture(tex0, texCoord) * color;
}