#version 330 core
out vec4 FragColor;

// per-fragment positon and normal passed from vertex shader
in vec3 normal;
in vec3 position;

uniform vec3 color;
uniform vec3 ambientLight;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	FragColor = vec4((diffuse + ambientLight)*color, 1.0);
}
