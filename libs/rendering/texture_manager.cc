// (C) 2017 Maciej Gajewski
#include "texture_manager.hh"

#include "texture.hh"

#include <cassert>

namespace Rendering {

TextureManager::TextureManager()
{
}

Texture* TextureManager::Load(const std::string& name, Texture::MipmapsOption mipmaps)
{
	auto it = mTextures.find(name);

	if (it == mTextures.end())
	{
		Texture t;
		t.loadFromFile(name, mipmaps);
		auto res = mTextures.emplace(name, std::move(t));
		assert(res.second);
		it = res.first;
	}

	return &(it->second);
}


} // namespace Rednering
