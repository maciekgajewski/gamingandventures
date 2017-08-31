#include "shader.hh"

#include <glad/glad.h>

#include <cassert>
#include <iostream>

namespace OT {

static unsigned loadShader(const std::string& source, unsigned shaderType)
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
		std::cout << "Error compiling shader " << std::endl;
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


Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	unsigned vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
	unsigned fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	shaderProgramId_ = linkShaders(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(shaderProgramId_);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgramId_);
}

}
