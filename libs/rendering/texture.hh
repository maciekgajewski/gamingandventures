#pragma once

#include <string>
#include <vector>

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

	void createEmpty(int w, int h);
	void loadFromFile(const std::string& path, MipmapsOption generateMipmaps);

	void loadCubemap(const std::vector<std::string>& paths);

public: // renderer API

	unsigned glId() { return textureId_; }

private:

	void create(int w, int h);

	unsigned textureId_ = 0;
	int width_ = 0;
	int height_ = 0;
};

} // namespace OT

