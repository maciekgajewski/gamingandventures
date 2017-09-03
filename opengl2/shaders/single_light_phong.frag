#version 330 core
out vec4 FragColor;

// per-fragment positon and normal passed from vertex shader
in vec3 normal;
in vec3 position;
in vec2 textureCoord;

uniform vec3 ambientLight;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D diffuseTexture;

void main()
{
	vec4 color = texture(diffuseTexture, textureCoord);

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	FragColor = vec4((diffuse + ambientLight)*color.rgb, 1.0);
	//FragColor = vec4(textureCoord, 0.00001*ambientLight.x*lightPos.y*lightColor.z, 0.0);
}