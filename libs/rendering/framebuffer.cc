// (C) 2017 Maciej Gajewski
#include "framebuffer.hh"

#include "texture.hh"
#include "renderer.hh"

#include <glad/glad.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cassert>
#include <memory>

namespace Rendering {

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &framebufferId_);
}

Framebuffer::~Framebuffer()
{
	if (framebufferId_)
		glDeleteFramebuffers(1, &framebufferId_);
}

void Framebuffer::AttachColorBuffer(Texture& texture)
{
	assert(texture.glId() && "Texture not initialized");
	assert(framebufferId_ && "Framebuffer not initilized");

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferId_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.glId(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AttachDepthBuffer(Texture& texture)
{
	// TODO
}

void Framebuffer::SaveToFile(const std::string& path, int x, int y, int w, int h)
{
	if(path.length() < 5)
		throw std::runtime_error("Invalid file name");

	unsigned bufsize = w*h*4;
	std::unique_ptr<char[]> buf(new char[bufsize]);

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferId_);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf.get());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::string ext = path.substr(path.length() - 4);
	if (ext == ".png")
	{
		int res = stbi_write_png(path.c_str(), w, h, 4, buf.get(), 0);
		if (res == 0)
			throw std::runtime_error("Error writing image");
	}
	else
		throw std::runtime_error("Unsupported image format");
}

} // namespace Rendering
