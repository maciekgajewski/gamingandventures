// (C) 2017 Maciej Gajewski
#pragma once

#include <memory>

namespace Ecs {
	class Ecs;
}

class RenderingSystem;

class InputSystem
{
public:
	InputSystem(RenderingSystem& rs, Ecs::Ecs& database);
	~InputSystem();

	void Init();
	void SetViewport(int x, int y, int w, int h);

	void OnCursorMove(double x, double y);

	void PollEvents();

private:

	RenderingSystem& renderingSystem_;
	Ecs::Ecs& database_;

	int w_ = 0;
	int h_ = 0;
	int x_ = 0;
	int y_ = 0;
};

