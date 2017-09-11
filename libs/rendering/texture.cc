#include "texture.hh"

#include "renderer.hh"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

#include <stdexcept>

namespace Rendering {

Texture::Texture()
{

}

Texture::~Texture()
{
	if (textureId_)
		glDeleteTextures(1, &textureId_);
}

Texture::Texture(Texture&& other)
{
	std::swap(textureId_, other.textureId_);
	std::swap(width_, other.width_);
	std::swap(height_, other.height_);
}

void Texture::createEmpty(int width, int height)
{
	create(width, height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	Renderer::checkError();
}

void Texture::generateSolid(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
	create(1, 1);
	std::uint32_t data = (a<<24) + (r<<16) + (g<<8) + b;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	Renderer::checkError();
}

void Texture::create(int width, int height)
{
	assert(textureId_ == 0 && "texture already initialized");

	width_ = width;
	height_ = height;

	glGenTextures(1, &textureId_);
	Renderer::checkError();
	glBindTexture(GL_TEXTURE_2D, textureId_);
	Renderer::checkError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	Renderer::checkError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Renderer::checkError();
	// TODO enable anisotropy here

}

void Texture::loadFromFile(const std::string& path, Texture::MipmapsOption generateMipmaps)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw std::runtime_error("Failed to load image '" + path + "' : " + stbi_failure_reason());

	create(width, height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	Renderer::checkError();

	if (generateMipmaps == GENERATE_MIPMAPS)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		Renderer::checkError();
	}

	stbi_image_free(data);
}

void Texture::loadCubemap(const std::vector<std::string>& paths)
{
	assert(textureId_ == 0 && "texture already initialized");

	assert (paths.size() == 6);

	glGenTextures(1, &textureId_);
	Renderer::checkError();
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
	Renderer::checkError();

	int nrChannels;
	for (int i = 0; i < 6; i++)
	{
		unsigned char *data = stbi_load(paths[i].c_str(), &width_, &height_, &nrChannels, 0);
		if (data)
		{
			if (nrChannels == 3)
			{
				glTexImage2D(
						GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					);
				Renderer::checkError();
			}
			else
			{
				throw std::runtime_error("Unsopported format of " + paths[i]);
			}
			stbi_image_free(data);
		}
		else
		{
			throw std::runtime_error("Error loading file: " + paths[i]);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	Renderer::checkError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Renderer::checkError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	Renderer::checkError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	Renderer::checkError();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	Renderer::checkError();
}

} // namespace OT
