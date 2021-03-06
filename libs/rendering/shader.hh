#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <string>


namespace Rendering {

class Uniform
{
public:

	Uniform() = default;
	explicit Uniform(int l) : location_(l) {}

	void set(const glm::vec3& v)
	{
		glUniform3f(location_, v.x, v.y, v.z);
	}

	void set(const glm::vec4& v)
	{
		glUniform4f(location_, v.r, v.g, v.b, v.a);
	}

	void set(const glm::mat4& m)
	{
		glUniformMatrix4fv(location_, 1, GL_FALSE, glm::value_ptr(m));
	}

	void set(std::int32_t v)
	{
		glUniform1i(location_, v);
	}

	void set(float v)
	{
		glUniform1f(location_, v);
	}

private:

	int location_ = -1;
};

class Shader
{
public:
	Shader() = default;
	Shader(
		const std::string& vertexShaderSource,
		const std::string& vertexShaderName,
		const std::string& fragmentShaderSource,
		const std::string& fragmentShaderName
		);
	Shader(const Shader&) = delete;
	Shader(Shader&& other) { std::swap(shaderProgramId_, other.shaderProgramId_); }
	~Shader();

	// Manipulating uniforms
	Uniform getUniform(const char* name) const;

	template<typename T>
	void setUniform(const char* name, const T& v)
	{
		getUniform(name).set(v);
	}

	Shader& operator=(Shader&& other) { std::swap(shaderProgramId_, other.shaderProgramId_); return *this; }
	Shader& operator=(const Shader&) = delete;

public: // Renderer access

	unsigned glId() const { return shaderProgramId_; }

private:

	unsigned shaderProgramId_ = 0;
};

}
