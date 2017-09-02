#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <string>


namespace OT {

class Uniform
{
public:

	Uniform() = default;
	explicit Uniform(int l) : location_(l) {}

	void Set(const glm::vec3& v)
	{
		glUniform3f(location_, v.x, v.y, v.z);
	}

	void Set(const glm::mat4& m)
	{
		glUniformMatrix4fv(location_, 1, GL_FALSE, glm::value_ptr(m));
	}


private:

	int location_ = -1;
};

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	Shader(const Shader&) = delete;
	Shader(Shader&& other) { std::swap(shaderProgramId_, other.shaderProgramId_); }
	~Shader();

	void Use();
	Uniform GetUniform(const char* name) const;


	Shader& operator=(Shader&& other) { std::swap(shaderProgramId_, other.shaderProgramId_); return *this; }
	Shader& operator=(const Shader&) = delete;

private:

	unsigned shaderProgramId_ = 0;
};

}
