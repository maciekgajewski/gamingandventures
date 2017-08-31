#pragma once

#include <string>


namespace OT {

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	Shader(const Shader&) = delete;
	~Shader();

	void Use();

	Shader& operator=(Shader&& other) { std::swap(shaderProgramId_, other.shaderProgramId_); return *this; }
	Shader& operator=(const Shader&) = delete;

private:

	unsigned shaderProgramId_ = 0;
};

}
