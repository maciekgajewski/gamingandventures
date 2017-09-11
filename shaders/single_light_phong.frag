#version 330 core
out vec4 FragColor;

// per-fragment positon and normal passed from vertex shader
in vec3 normal;
in vec3 position;
in vec2 textureCoord;

uniform vec3 ambientLight;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform float shininess;
uniform float opacity;

uniform sampler2D diffuseTexture;

void main()
{
	vec4 color = texture(diffuseTexture, textureCoord);

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular
	vec3 viewDir = normalize(viewPos - position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = spec * lightColor;

	vec3 finalColor = vec3((diffuse + ambientLight + specular)*color.rgb);
	FragColor = vec4(clamp(finalColor, 0.0, 1.0), opacity);
}
