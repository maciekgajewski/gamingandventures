#pragma once

#include <string>

namespace Rendering {

class Texture
{
public:

	enum MipmapsOption { GENERATE_MIPMAPS, DONT_GENERATE_MIPMAPS };

	Texture();

	void LoadFromFile(const std::string& path, MipmapsOption generateMipmaps);

public: // renderer API

	unsigned glId() { return textureId_; }

private:

	unsigned textureId_ = 0;
	int width_ = 0;
	int height_ = 0;
};

} // namespace OT

