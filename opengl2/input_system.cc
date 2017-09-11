// (C) 2017 Maciej Gajewski
#include "input_system.hh"

#include "rendering_system.hh"
#include "components.hh"

#include <ecs/visitor.hh>

#include <GLFW/glfw3.h>

#include <iostream>

InputSystem::InputSystem(RenderingSystem& rs, Ecs::Ecs& database)
	: renderingSystem_(rs), database_(database)
{

}

InputSystem::~InputSystem()
{
}

void InputSystem::Init()
{
}

void InputSystem::SetViewport(int x, int y, int w, int h)
{
	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h;
}

void InputSystem::OnCursorMove(double x, double y)
{
	auto pick = renderingSystem_.QueryPickMap(x, y);
	//std::cout << "x=" << x << ", y=" << y << ", pick=" << pick << std::endl;
	auto visitor  = Ecs::buildAutoTypeVisitor<
		Components::MousePickable, Components::Selectable>(database_);

	visitor.forEach([&](Ecs::EntityId id, const Components::MousePickable& pickable, Components::Selectable& selectable)
	{
		if (pickable.value == pick)
			selectable.selected = true;
		else
			selectable.selected = false;
	});
}

void InputSystem::PollEvents()
{
	renderingSystem_.RenderPickMap();
	glfwPollEvents();
}

