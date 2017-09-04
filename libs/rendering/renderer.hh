// (C) 2017 Maciej Gajewski
#pragma once

#include <glm/glm.hpp>

namespace Rendering {

class Shader;
class Texture;

// Renderer is simple rendering API, which one can use to implement rendering pipeline
class Renderer
{
public:
	Renderer();

	void SetClearColor(const glm::vec3& color);

	enum class ClearedBuffers { Color, Depth, ColorDepth };
	void ClearBuffers(ClearedBuffers b);

	void SetDepthTest(bool enabled);

	void SetWireframeMode(bool wireframe);

	void UseShader(Shader& shader);
	void ActivateTexture(Texture& unit);

private:

};

} // namespace Rendering

