// (C) 2017 Maciej Gajewski
#include "renderer.hh"

namespace Rendering {

Renderer::Renderer()
{
}

void Renderer::SetClearColor(const glm::vec3& color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
}

} // namespace Rendering
