// (C) 2017 Maciej Gajewski
#pragma once

#include <ecs/visitor.hh>

#include <glm/glm.hpp>

#include <rendering/camera.hh>

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
	void Render();

	// Settings
	void SetCamera(const Rendering::Camera& cam);
	void SetCameraAspectRatio(float aspectRatio);
	void SetAmbientLight(const glm::vec3& color) { ambientLight_ = color; }
	void SetPointLight(const glm::vec3& position, const glm::vec3& color)
	{
		pointLightPos_ = position;
		pointLightColor_ = color;
	}

private:

	Rendering::Renderer& renderer_;
	Ecs::Ecs& data_;

	Rendering::Shader* solidShader_ = nullptr;

	Rendering::Camera camera_;
	glm::mat4 projectionTrans_;

	// our poor lightining system
	glm::vec3 ambientLight_;
	glm::vec3 pointLightPos_;
	glm::vec3 pointLightColor_;

};

