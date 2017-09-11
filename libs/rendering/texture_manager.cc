// (C) 2017 Maciej Gajewski
#include "texture_manager.hh"

#include "texture.hh"

#include <cassert>
#include <cstdio>

namespace Rendering {

TextureManager::TextureManager()
{
}

Texture* TextureManager::load(const std::string& name, Texture::MipmapsOption mipmaps)
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

static float clamp(float f)
{
	if (f < 0.0f) return 0.0f;
	if (f > 1.0f) return 1.0f;
	return f;
}

Texture* TextureManager::getSolid(const glm::vec4& rgba)
{
	std::uint8_t r(clamp(rgba.r) * 255);
	std::uint8_t g(clamp(rgba.g) * 255);
	std::uint8_t b(clamp(rgba.b) * 255);
	std::uint8_t a(clamp(rgba.a) * 255);

	char buf[32];
	std::snprintf(buf, 32, "solid:0x%02x%02x%02x%02x", a, r, g, b);
	auto it = mTextures.find(buf);

	if (it == mTextures.end())
	{
		Texture t;
		t.generateSolid(r, g, b, a);
		auto res = mTextures.emplace(buf, std::move(t));
		assert(res.second);
		it = res.first;
	}

	return &(it->second);
}


} // namespace Rednering
