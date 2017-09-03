#include "shader.hh"

#include <glad/glad.h>

#include <cassert>
#include <iostream>

namespace OT {

using namespace std::literals;

static unsigned loadShader(const std::string& source, unsigned shaderType, const std::string& name)
{
	assert(shaderType == GL_FRAGMENT_SHADER || shaderType == GL_VERTEX_SHADER);

	unsigned shaderId = glCreateShader(shaderType);
	const char* codePtr = source.c_str();
	glShaderSource(shaderId, 1, &codePtr, nullptr);
	glCompileShader(shaderId);

	int  success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "Error compiling shader " << name << std::endl;
		std::cout << infoLog << std::endl;
		throw std::runtime_error("Error compiling shader");
	}

	return shaderId;
}

static unsigned linkShaders(unsigned vertexShader, unsigned fragmentShader)
{
	unsigned sp = glCreateProgram();
	glAttachShader(sp, vertexShader);
	glAttachShader(sp, fragmentShader);
	glLinkProgram(sp);

	int  success;
	char infoLog[512];
	glGetProgramiv(sp, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(sp, 512, NULL, infoLog);
		std::cout << "Error linking shaders" << std::endl;
		std::cout << infoLog << std::endl;
		throw std::runtime_error("Error linking shaders");
	}
	return sp;
}


Shader::Shader(
		const std::string& vertexShaderSource,
		const std::string& vertexShaderName,
		const std::string& fragmentShaderSource,
		const std::string& fragmentShaderName)
{
	unsigned vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShaderName);
	unsigned fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShaderName);
	shaderProgramId_ = linkShaders(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}



void Shader::Use()
{
	assert(shaderProgramId_ && "Shader not initialized");
	glUseProgram(shaderProgramId_);
}

Uniform Shader::GetUniform(const char* name) const
{
	assert(shaderProgramId_ && "Shader not initialized");
	int loc = glGetUniformLocation(shaderProgramId_, name);
	if (loc == -1)
		throw std::logic_error("No uniform '"s + name + "' found in shader");

	return Uniform(loc);
}

Shader::~Shader()
{
	if (shaderProgramId_)
		glDeleteProgram(shaderProgramId_);
}

}
