#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexture;

// per-fragment positon and normal passed to fragmet shader
out vec3 normal;
out vec3 position;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position = projection * camera * model * vec4(inPos, 1.0);

	normal = mat3(transpose(inverse(model))) * inNormal;
	position = vec3(model * vec4(inPos, 1.0));
	textureCoord = inTexture;
}
