// (C) 2017 Maciej Gajewski
#include "rendering_system.hh"

#include <rendering/renderer.hh>
#include <rendering/components.hh>

#include <glm/gtc/matrix_transform.hpp>

RenderingSystem::RenderingSystem(Rendering::Renderer& renderer, Ecs::Ecs& data)
	: renderer_(renderer), data_(data)
{

}

void RenderingSystem::Init()
{
	solidShader_ = renderer_.Shaders().Load("shaders/single_light_phong.vert", "shaders/single_light_phong.frag");

	ambientLight_ = glm::vec3(1.0f);
	pointLightPos_ = glm::vec3(0.0f);
	pointLightColor_ = glm::vec3(0.0f);

	renderer_.SetClearColor(glm::vec3(0.05));

	data_.RegisterUniqueComponentType<Rendering::Components::Material>("Material");
	data_.RegisterUniqueComponentType<Rendering::Components::Mesh>("Mesh");
	data_.RegisterUniqueComponentType<Rendering::Components::Transformation>("Transformation");
}


void RenderingSystem::Render()
{
	// TODO
}

void RenderingSystem::SetCamera(const Rendering::Camera& cam)
{
	camera_ = cam;
}

void RenderingSystem::SetCameraAspectRatio(float aspectRatio)
{
	projectionTrans_ = glm::perspective(glm::radians(45.0f),
			aspectRatio,
			0.1f, // near
			1000.0f // far
			);
}

