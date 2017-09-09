// (C) 2017 Maciej Gajewski
#include "renderer.hh"

#include "framebuffer.hh"

namespace Rendering {

Renderer::Renderer()
{
}

void Renderer::SetClearColor(const glm::vec3& color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::SetViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void Renderer::ClearBuffers(Renderer::ClearedBuffers b)
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
}

void Renderer::SetDepthTest(bool enabled)
{
	if (enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void Renderer::SetFaceCulling(bool enabled)
{
	if (enabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Renderer::SetWireframeMode(bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Renderer::UseShader(Shader& shader)
{
	unsigned programId = shader.glId();
	assert(programId && "Shader not initialized");
	glUseProgram(programId);
}

void Renderer::ActivateTexture(Texture& texture, unsigned unit)
{
	unsigned textureId = texture.glId();

	assert(unit >=0 && unit < 16 && "Texture unit out of range");
	assert(textureId > 0 && "Texture not initialized");

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureId);

}

void Renderer::RenderToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderTo(Framebuffer& fb)
{
	unsigned fbId = fb.glId();

	assert(fbId && "Framebuffer not initialized");
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Framebuffer not complete");

	glBindFramebuffer(GL_FRAMEBUFFER, fbId);
}

void Renderer::CheckError()
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
