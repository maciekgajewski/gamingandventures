// (C) 2017 Maciej Gajewski
#pragma once

#include "texture.hh"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <memory>

namespace Rendering {

class Texture;


// Texture Manager manages the texture resources.
//
// All textures are owned by this class. It is reponsible for creating and loading textures
class TextureManager
{
public:
	TextureManager();

	// Loads texture from file. Once loaded, the texture stays in cache, so that subsequent calls will be served from the cache
	Texture* load(const std::string& name, Texture::MipmapsOption mipmaps = Texture::GENERATE_MIPMAPS);

	// Creates 1x1 texture with supplied color
	Texture* getSolid(const glm::vec4& rgba);

private:

	std::unordered_map<std::string, Texture> mTextures;
};

} // namespace Rednering

