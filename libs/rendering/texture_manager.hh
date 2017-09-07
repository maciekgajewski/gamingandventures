// (C) 2017 Maciej Gajewski
#pragma once

#include "texture.hh"

#include <string>
#include <unordered_map>

namespace Rendering {

class Texture;


// Texture Manager manages the texture resources.
//
// All textures are owned by this class. It is reponsible for creating and loading textures
class TextureManager
{
public:
	TextureManager();

	Texture* Load(const std::string& name, Texture::MipmapsOption mipmaps = Texture::GENERATE_MIPMAPS);

	// TODO
	// Texture* Create(w, h, format);

private:

	std::unordered_map<std::string, Texture> mTextures;
};

} // namespace Rednering

