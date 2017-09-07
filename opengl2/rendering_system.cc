// (C) 2017 Maciej Gajewski
#include "rendering_system.hh"

#include <rendering/renderer.hh>
#include <rendering/components.hh>

#include <glm/gtc/matrix_transform.hpp>

RenderingSystem::RenderingSystem(Rendering::Renderer& renderer, Ecs::Ecs& data)
	: renderer_(renderer), database_(data)
{

}

void RenderingSystem::Init()
{
	solidShader_ = renderer_.Shaders().Load("shaders/single_light_phong.vert", "shaders/single_light_phong.frag");

	ambientLight_ = glm::vec3(1.0f);
	pointLightPos_ = glm::vec3(0.0f);
	pointLightColor_ = glm::vec3(0.0f);

	renderer_.SetClearColor(glm::vec3(0.05));
	renderer_.SetDepthTest(true);
	renderer_.SetWireframeMode(false);
	renderer_.SetFaceCulling(true);

	database_.RegisterUniqueComponentType<Rendering::Components::Material>("Material");
	database_.RegisterUniqueComponentType<Rendering::Components::Mesh>("Mesh");
	database_.RegisterUniqueComponentType<Rendering::Components::Transformation>("Transformation");
}


void RenderingSystem::Render()
{
	renderer_.ClearBuffers(Rendering::Renderer::ClearedBuffers::ColorDepth);

	renderer_.UseShader(*solidShader_);

	solidShader_->SetUniform("ambientLight", ambientLight_);
	solidShader_->SetUniform("lightColor", pointLightColor_);
	solidShader_->SetUniform("lightPos", pointLightPos_);

	solidShader_->SetUniform("camera", camera_.CalculateTransformation());
	solidShader_->SetUniform("projection", projectionTrans_);
	solidShader_->SetUniform("viewPos", camera_.GetPosition());

	Rendering::Uniform modelUniform = solidShader_->GetUniform("model");

	// Iterate
	auto visitor = Ecs::BuildUniqueTypeVisitor< // TODO move to Init
		Rendering::Components::Material,
		Rendering::Components::Mesh,
		Rendering::Components::Transformation>(database_);

	visitor.ForEach([&](
		Ecs::EntityId id,
		const Rendering::Components::Material& material,
		const Rendering::Components::Mesh& mesh,
		const Rendering::Components::Transformation& trans)
		{
			modelUniform.Set(trans.transformation);

			renderer_.ActivateTexture(*material.diffuseTexture, 0);
			solidShader_->SetUniform("shininess", material.shininess);

			mesh.mesh->Draw();
		});

}

void RenderingSystem::SetViewport(int x, int y, int w, int h)
{
	renderer_.SetViewport(x, y, w, h);
	projectionTrans_ = glm::perspective(glm::radians(45.0f),
			float(w)/h,
			0.1f, // near
			1000.0f // far
			);
}

void RenderingSystem::SetCamera(const Rendering::Camera& cam)
{
	camera_ = cam;
}

