#version 330 core
out vec4 FragColor;

in vec3 vertexColor;

void main()
{
	FragColor = vec4(vertexColor.xyz, 1.0f);
}
