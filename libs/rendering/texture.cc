#include "texture.hh"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

#include <stdexcept>

namespace Rendering {

Texture::Texture()
{

}

void Texture::LoadFromFile(const std::string& path, Texture::MipmapsOption generateMipmaps)
{
	assert(textureId_ == 0 && "texture already initialized");

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
		throw std::runtime_error("Failed to load image '" + path + "' : " + stbi_failure_reason());

	width_ = width;
	height_ = height;

	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// TODO enable anisotropy here

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	if (generateMipmaps == GENERATE_MIPMAPS)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}

void Texture::Activate(int unit) const
{
	assert(unit >=0 && unit < 16 && "Texture unit out of range");
	assert(textureId_ > 0 && "Texture not initialized");

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureId_);
}

} // namespace OT
