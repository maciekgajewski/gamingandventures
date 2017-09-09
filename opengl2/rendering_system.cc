// (C) 2017 Maciej Gajewski
#include "rendering_system.hh"

#include "components.hh"

#include <rendering/renderer.hh>
#include <rendering/components.hh>
#include <rendering/framebuffer.hh>

#include <glm/gtc/matrix_transform.hpp>


RenderingSystem::RenderingSystem(Rendering::Renderer& renderer, Ecs::Ecs& data)
	: renderer_(renderer), database_(data)
{
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::Init()
{
	solidShader_ = renderer_.Shaders().Load("shaders/single_light_phong.vert", "shaders/single_light_phong.frag");
	pickShader_ = renderer_.Shaders().Load("shaders/solid.vert", "shaders/solid.frag");

	ambientLight_ = glm::vec3(1.0f);
	pointLightPos_ = glm::vec3(0.0f);
	pointLightColor_ = glm::vec3(0.0f);

	database_.RegisterUniqueComponentType<Rendering::Components::Material>("Material");
	database_.RegisterUniqueComponentType<Rendering::Components::Mesh>("Mesh");
	database_.RegisterUniqueComponentType<Rendering::Components::Transformation>("Transformation");
	database_.RegisterUniqueComponentType<Components::MousePickable>("MousePickable");
	database_.RegisterUniqueComponentType<Components::Selectable>("Selectable");

	offScreen_ = std::make_unique<Rendering::Framebuffer>();
}


void RenderingSystem::Render()
{
	renderer_.RenderToScreen();
	DoRender();
}

void RenderingSystem::DoRender()
{
	renderer_.SetDepthTest(true);
	renderer_.SetWireframeMode(false);
	renderer_.SetFaceCulling(true);

	renderer_.SetClearColor(glm::vec3(0.05));
	renderer_.ClearBuffers(Rendering::Renderer::ClearedBuffers::ColorDepth);
	renderer_.UseShader(*solidShader_);

	solidShader_->SetUniform("ambientLight", ambientLight_);
	solidShader_->SetUniform("lightColor", pointLightColor_);
	solidShader_->SetUniform("lightPos", pointLightPos_);

	solidShader_->SetUniform("camera", camera_.CalculateTransformation());
	solidShader_->SetUniform("projection", projectionTrans_);
	solidShader_->SetUniform("viewPos", camera_.GetPosition());

	Rendering::Uniform modelUniform = solidShader_->GetUniform("model");

	// Iterate and render
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

void RenderingSystem::RenderToFile()
{
	renderer_.RenderTo(*offScreen_);
	DoRender();
	offScreen_->SaveToFile("out.png", 0, 0, width_, height_);
}

void RenderingSystem::RenderPickMap()
{
	renderer_.SetDepthTest(true);
	renderer_.SetWireframeMode(false);
	renderer_.SetFaceCulling(true);
	renderer_.RenderTo(*offScreen_);

	renderer_.SetClearColor(glm::vec3(0.0f));

	renderer_.ClearBuffers(Rendering::Renderer::ClearedBuffers::ColorDepth);
	renderer_.UseShader(*pickShader_);

	pickShader_->SetUniform("camera", camera_.CalculateTransformation());
	pickShader_->SetUniform("projection", projectionTrans_);

	Rendering::Uniform modelUniform = pickShader_->GetUniform("model");

	// Iterate
	auto visitor = Ecs::BuildUniqueTypeVisitor< // TODO move to Init
		Components::MousePickable,
		Rendering::Components::Transformation>(database_);

	visitor.ForEach([&](
		Ecs::EntityId id,
		const Components::MousePickable& pickable,
		const Rendering::Components::Transformation& trans)
		{
			pickShader_->SetUniform("model", trans.transformation);

			union rgba
			{
				std::uint32_t v;
				struct {
					std::uint8_t r, g, b, a;
				};
			};

			rgba u;
			u.v = pickable.value;

			glm::vec4 color(u.r/255.0, u.g/255.0, u.b/255.0, u.a/255.0);

			pickShader_->SetUniform("color", color);

			pickable.mesh->Draw();
		});

	offScreen_->SaveToFile("pickmap.png", 0, 0, width_, height_);
}


void RenderingSystem::SetViewport(int x, int y, int w, int h)
{
	renderer_.SetViewport(x, y, w, h);
	projectionTrans_ = glm::perspective(glm::radians(45.0f),
			float(w)/h,
			0.1f, // near
			1000.0f // far
			);

	offScreenColor_ = std::make_unique<Rendering::Texture>();
	offScreenColor_->CreateEmpty(w, h);
	offScreen_->AttachColorBuffer(*offScreenColor_);
	width_ = w;
	height_ = h;
}

void RenderingSystem::SetCamera(const Rendering::Camera& cam)
{
	camera_ = cam;
}

