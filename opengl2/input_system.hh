// (C) 2017 Maciej Gajewski
#pragma once

#include <memory>

namespace Rendering {
	class Renderer;
	class Shader;
	class Texture;
	class Framebuffer;
	class Mesh;
}

namespace Ecs {
	class Ecs;
}


class InputSystem
{
public:
	InputSystem(Rendering::Renderer& renderer, Ecs::Ecs& database);
	~InputSystem();

	void Init();
	void SetViewport(int x, int y, int w, int h);

	void OnCursorMove(double x, double y);

	void PollEvents();

private:

	void DrawPickMap();

	Rendering::Renderer& renderer_;
	Ecs::Ecs& database_;

	int w_ = 0;
	int h_ = 0;
	int x_ = 0;
	int y_ = 0;

	std::unique_ptr<Rendering::Texture> offScreenColor_;
	std::unique_ptr<Rendering::Framebuffer> offScreen_;
	Rendering::Shader* pickShader_ = nullptr;
};

