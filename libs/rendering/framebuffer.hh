// (C) 2017 Maciej Gajewski
#pragma once

#include <string>

namespace Rendering {

class Texture;

// Off-screen render target
class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;

	void AttachColorBuffer(Texture& texture);
	void AttachDepthBuffer(Texture& texture);

	void SaveToFile(const std::string& path, int x, int y, int w, int h) const;
	std::uint32_t QueryPixel(int x, int y) const;

public: // Renderer API

	unsigned glId() { return framebufferId_; }

private:

	unsigned framebufferId_ = 0;
};

} // namespace Rendering

