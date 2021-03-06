// (C) 2017 Maciej Gajewski
#pragma once

#include <ecs/visitor.hh>

#include <glm/glm.hpp>

#include <rendering/camera.hh>

#include <memory>

namespace Rendering {
	class Renderer;
	class Shader;
	class Texture;
	class Framebuffer;
	class Mesh;
}

// This application's rendering system
class RenderingSystem
{
public:
	RenderingSystem(Rendering::Renderer& renderer, Ecs::Ecs& data);
	~RenderingSystem();

	// Loads all resources required by the system
	void Init();

	// Renders a frame
	void Render();
	void RenderToFile();
	void RenderPickMap();
	std::uint32_t QueryPickMap(int x, int y) const;

	// Settings
	void SetViewport(int x, int y, int w, int h);
	void setCamera(const Rendering::Camera& cam);
	Rendering::Camera getCamera() const { return camera_; }
	void SetAmbientLight(const glm::vec3& color) { ambientLight_ = color; }
	void SetPointLight(const glm::vec3& position, const glm::vec3& color)
	{
		pointLightPos_ = position;
		pointLightColor_ = color;
	}

private:

	void DoRender();

	Rendering::Renderer& renderer_;
	Ecs::Ecs& database_;

	Rendering::Shader* solidShader_ = nullptr;
	Rendering::Shader* pickShader_ = nullptr;

	Rendering::Camera camera_;
	glm::mat4 projectionTrans_;

	// our poor lightining system
	glm::vec3 ambientLight_;
	glm::vec3 pointLightPos_;
	glm::vec3 pointLightColor_;

	int width_ = 0;
	int height_ = 0;

	// off-screen buffer
	std::unique_ptr<Rendering::Texture> offScreenColor_;
	std::unique_ptr<Rendering::Framebuffer> offScreen_;

	// skybox
	std::unique_ptr<Rendering::Mesh> skyboxMesh_;
	std::unique_ptr<Rendering::Texture>  skybox_;
	Rendering::Shader* skyboxShader_ = nullptr;

};

