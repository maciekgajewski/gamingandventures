// (C) 2017 Maciej Gajewski
#pragma once

#include <ecs/visitor.hh>

namespace Rendering {
class Renderer;
class Shader;
}

// This application's rendering system
class RenderingSystem
{
public:
	RenderingSystem(Rendering::Renderer& renderer, Ecs::Ecs& data);

	// Loads all resources required by the system
	void Init();

	// Renders a frame
	void Draw();

private:

	Rendering::Renderer& renderer_;
	Ecs::Ecs& data_;

	Rendering::Shader* solidShader_ = nullptr;

};

