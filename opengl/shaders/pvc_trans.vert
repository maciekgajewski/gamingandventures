#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec4 vertexColor;

uniform mat4 trans;

void main()
{
	gl_Position = trans * vec4(pos, 1.0);
	vertexColor = trans * vec4(color, 1.0);
}
