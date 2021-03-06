#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position = projection * camera * model * vec4(pos, 1.0);
	vertexColor = vec4(color, 1.0);
}
