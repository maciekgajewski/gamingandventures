#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 camera;
uniform mat4 projection;

void main()
{
	TexCoords = aPos;
	gl_Position = projection * camera * vec4(aPos, 1.0);
}
