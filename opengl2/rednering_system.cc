// (C) 2017 Maciej Gajewski
#include "rednering_system.hh"

#include <rendering/renderer.hh>

RenderingSystem::RenderingSystem(Rendering::Renderer& renderer, Ecs::Ecs& data)
	: renderer_(renderer), data_(data)
{

}

void RenderingSystem::Init()
{
	solidShader_ = renderer_.Shaders().Load("shaders/single_light_phong.vert", "shaders/single_light_phong.frag");
}

void RenderingSystem::Draw()
{
	// TODO
}
