#pragma once

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <string>


namespace OT {

class Uniform
{
public:

	explicit Uniform(int l) : location_(l) {}

	void Set(const glm::vec3& v)
	{
		glUniform3f(location_, v.x, v.y, v.z);
	}


private:

	int location_ = 0;
};

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	Shader(const Shader&) = delete;
	~Shader();

	void Use();
	Uniform GetUniform(const char* name) const;


	Shader& operator=(Shader&& other) { std::swap(shaderProgramId_, other.shaderProgramId_); return *this; }
	Shader& operator=(const Shader&) = delete;

private:

	unsigned shaderProgramId_ = 0;
};

}
