#pragma once

#include <string>

namespace Rendering {

class Texture
{
public:

	enum MipmapsOption { GENERATE_MIPMAPS, DONT_GENERATE_MIPMAPS };

	Texture();
	~Texture();

	Texture(Texture&& other);

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void CreateEmpty(int w, int h);
	void LoadFromFile(const std::string& path, MipmapsOption generateMipmaps);

public: // renderer API

	unsigned glId() { return textureId_; }

private:

	void Create(int w, int h);

	unsigned textureId_ = 0;
	int width_ = 0;
	int height_ = 0;
};

} // namespace OT

