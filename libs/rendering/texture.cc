#include "texture.hh"

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
}

void Texture::CreateEmpty(int width, int height)
{
	Create(width, height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Texture::Create(int width, int height)
{
	assert(textureId_ == 0 && "texture already initialized");

	width_ = width;
	height_ = height;

	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// TODO enable anisotropy here

}

void Texture::LoadFromFile(const std::string& path, Texture::MipmapsOption generateMipmaps)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw std::runtime_error("Failed to load image '" + path + "' : " + stbi_failure_reason());

	Create(width, height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	if (generateMipmaps == GENERATE_MIPMAPS)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}

} // namespace OT
