#pragma once

#include <string>

namespace OT {

class Texture
{
public:

	enum MipmapsOption { GENERATE_MIPMAPS, DONT_GENERATE_MIPMAPS };

	Texture();

	void LoadFromFile(const std::string& path, MipmapsOption generateMipmaps);

	// Activates textue as specified sampler unit
	void Activate(int unit) const;

private:

	unsigned textureId_ = 0;
	int width_ = 0;
	int height_ = 0;
};

} // namespace OT

