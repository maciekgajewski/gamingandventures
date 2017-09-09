// (C) 2017 Maciej Gajewski
#include "input_system.hh"

#include <rendering/renderer.hh>
#include <rendering/framebuffer.hh>

#include <GLFW/glfw3.h>

#include <iostream>

InputSystem::InputSystem(Rendering::Renderer& renderer, Ecs::Ecs& database)
	: renderer_(renderer), database_(database)
{

}

InputSystem::~InputSystem()
{
}

void InputSystem::Init()
{
	pickShader_ = renderer_.Shaders().Load("shaders/solid.vert", "shaders/solid.frag");
	offScreen_ = std::make_unique<Rendering::Framebuffer>();
}

void InputSystem::SetViewport(int x, int y, int w, int h)
{
	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h;

	offScreenColor_ = std::make_unique<Rendering::Texture>();
	offScreenColor_->CreateEmpty(w, h);
	offScreen_->AttachColorBuffer(*offScreenColor_);
}

void InputSystem::OnCursorMove(double x, double y)
{
	std::cout << "x=" << x << ", y=" << y << std::endl;
	// TODO
}

void InputSystem::PollEvents()
{
	DrawPickMap();
	glfwPollEvents();
}

void InputSystem::DrawPickMap()
{
	// TODO
}
