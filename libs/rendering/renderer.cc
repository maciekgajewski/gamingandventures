// (C) 2017 Maciej Gajewski
#include "renderer.hh"

#include "framebuffer.hh"

namespace Rendering {

Renderer::Renderer()
{
}

void Renderer::setClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	checkError();
}

void Renderer::setViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
	checkError();
}

void Renderer::clearBuffers(Renderer::ClearedBuffers b)
{
	switch(b)
	{
		case ClearedBuffers::Color:
			glClear(GL_COLOR_BUFFER_BIT);
			break;
		case ClearedBuffers::Depth:
			glClear(GL_DEPTH_BUFFER_BIT);
			break;
		case ClearedBuffers::ColorDepth :
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			break;
	}
	checkError();
}

void Renderer::setDepthTest(bool enabled)
{
	if (enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	checkError();
}

void Renderer::setDepthMask(bool enabled)
{
	if (enabled)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);
	checkError();
}

void Renderer::setFaceCulling(bool enabled)
{
	if (enabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	checkError();
}

void Renderer::setWireframeMode(bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	checkError();

}

void Renderer::setBlend(bool enabled)
{
	if (enabled)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	checkError();

	if (enabled)
	{
		// expose, if needed
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		checkError();
	}
}

void Renderer::useShader(Shader& shader)
{
	unsigned programId = shader.glId();
	assert(programId && "Shader not initialized");
	glUseProgram(programId);
	checkError();
}

void Renderer::activateTexture(Texture& texture, unsigned unit)
{
	unsigned textureId = texture.glId();

	assert(unit >=0 && unit < 16 && "Texture unit out of range");
	assert(textureId > 0 && "Texture not initialized");

	glActiveTexture(GL_TEXTURE0 + unit);
	checkError();
	glBindTexture(GL_TEXTURE_2D, textureId);
	checkError();

}

void Renderer::activateCubemap(Texture& texture)
{
	unsigned textureId = texture.glId();

	assert(textureId > 0 && "Texture not initialized");

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	checkError();
}

void Renderer::renderToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	checkError();
}

void Renderer::renderTo(Framebuffer& fb)
{
	unsigned fbId = fb.glId();

	assert(fbId && "Framebuffer not initialized");
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Framebuffer not complete");

	glBindFramebuffer(GL_FRAMEBUFFER, fbId);
	checkError();
}

void Renderer::checkError()
{
	unsigned err = glGetError();

	switch(err)
	{
		case GL_INVALID_ENUM:
			throw std::runtime_error("GL_INVALID_ENUM");

		case GL_INVALID_VALUE:
			throw std::runtime_error("GL_INVALID_VALUE");

		case GL_INVALID_OPERATION:
			throw std::runtime_error("GL_INVALID_OPERATION");

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			throw std::runtime_error("GL_INVALID_FRAMEBUFFER_OPERATION");

		case GL_OUT_OF_MEMORY:
			throw std::runtime_error("GL_OUT_OF_MEMORY");

		case GL_NO_ERROR:
			return;
	}

	throw std::runtime_error("Unknown error " + std::to_string(err));
}

} // namespace Rendering
