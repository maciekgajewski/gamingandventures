#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform vec3 ambientLight;

void main()
{
	FragColor = vec4(ambientLight*color, 1.0);
}
