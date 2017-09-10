// (C) 2017 Maciej Gajewski
#include "rendering_system.hh"

#include "components.hh"

#include <rendering/renderer.hh>
#include <rendering/components.hh>
#include <rendering/framebuffer.hh>
#include <rendering/mesh_utilities.hh>

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
	solidShader_  = renderer_.shaders().Load("shaders/single_light_phong.vert", "shaders/single_light_phong.frag");
	pickShader_   = renderer_.shaders().Load("shaders/solid.vert", "shaders/solid.frag");
	skyboxShader_ = renderer_.shaders().Load("shaders/skybox.vert", "shaders/skybox.frag");

	ambientLight_ = glm::vec3(1.0f);
	pointLightPos_ = glm::vec3(0.0f);
	pointLightColor_ = glm::vec3(0.0f);

	database_.RegisterUniqueComponentType<Rendering::Components::Material>("Material");
	database_.RegisterUniqueComponentType<Rendering::Components::Mesh>("Mesh");
	database_.RegisterUniqueComponentType<Rendering::Components::Transformation>("Transformation");
	database_.RegisterUniqueComponentType<Components::MousePickable>("MousePickable");
	database_.RegisterUniqueComponentType<Components::Selectable>("Selectable");

	offScreen_ = std::make_unique<Rendering::Framebuffer>();

	skyboxMesh_ = std::make_unique<Rendering::Mesh>(Rendering::buildSkyboxMesh());
	skybox_ = std::make_unique<Rendering::Texture>();
//	skybox_->loadCubemap({
//			"textures/yokohama_cubemap/posx.jpg",
//			"textures/yokohama_cubemap/negx.jpg",
//			"textures/yokohama_cubemap/posy.jpg",
//			"textures/yokohama_cubemap/negy.jpg",
//			"textures/yokohama_cubemap/posz.jpg",
//			"textures/yokohama_cubemap/negz.jpg",
//		});
//	skybox_->loadCubemap({
//			"textures/mp_cloud9/cloud9_rt.tga",
//			"textures/mp_cloud9/cloud9_lf.tga",
//			"textures/mp_cloud9/cloud9_up.tga",
//			"textures/mp_cloud9/cloud9_dn.tga",
//			"textures/mp_cloud9/cloud9_bk.tga",
//			"textures/mp_cloud9/cloud9_ft.tga",
//		});
	skybox_->loadCubemap({
			"textures/mp_portal/portal_rt.tga",
			"textures/mp_portal/portal_lf.tga",
			"textures/mp_portal/portal_up.tga",
			"textures/mp_portal/portal_dn.tga",
			"textures/mp_portal/portal_bk.tga",
			"textures/mp_portal/portal_ft.tga",
		});
}


void RenderingSystem::Render()
{
	renderer_.renderToScreen();
	DoRender();
}

void RenderingSystem::DoRender()
{
	renderer_.setDepthTest(true);
	renderer_.setWireframeMode(false);
	renderer_.setFaceCulling(true);

	renderer_.setClearColor(glm::vec4(0.05, 0.05, 0.05, 0.0));
	renderer_.clearBuffers(Rendering::Renderer::ClearedBuffers::ColorDepth);

	// render skybox
	{
		Rendering::Camera fixedCam = camera_;
		fixedCam.setPosition(glm::vec3(0.0f));

		renderer_.useShader(*skyboxShader_);
		skyboxShader_->SetUniform("projection", projectionTrans_);
		skyboxShader_->SetUniform("camera", fixedCam.calculateTransformation());

		renderer_.activateCubemap(*skybox_);
		renderer_.setDepthMask(false);
		skyboxMesh_->Draw();
	}

	// render solid items
	renderer_.setDepthMask(true);
	renderer_.useShader(*solidShader_);

	solidShader_->SetUniform("ambientLight", ambientLight_);
	solidShader_->SetUniform("lightColor", pointLightColor_);
	solidShader_->SetUniform("lightPos", pointLightPos_);

	solidShader_->SetUniform("camera", camera_.calculateTransformation());
	solidShader_->SetUniform("projection", projectionTrans_);
	solidShader_->SetUniform("viewPos", camera_.getPosition());

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

			renderer_.activateTexture(*material.diffuseTexture, 0);
			solidShader_->SetUniform("shininess", material.shininess);

			mesh.mesh->Draw();
		});

	// Second pass - draw selection
	renderer_.setDepthTest(false);
	renderer_.useShader(*pickShader_);
	renderer_.setWireframeMode(true);

	pickShader_->SetUniform("camera", camera_.calculateTransformation());
	pickShader_->SetUniform("projection", projectionTrans_);
	pickShader_->SetUniform("color", glm::vec4(1.0, 0.8, 0.0, 1.0));

	auto selectionVisitor = Ecs::BuildUniqueTypeVisitor< // TODO move to Init
		Components::Selectable,
		Rendering::Components::Mesh,
		Rendering::Components::Transformation>(database_);

	selectionVisitor.ForEach([&](
		Ecs::EntityId id,
		const Components::Selectable& selectable,
		const Rendering::Components::Mesh& mesh,
		const Rendering::Components::Transformation& trans)
		{
			if (selectable.selected)
			{
				pickShader_->SetUniform("model", trans.transformation);
				mesh.mesh->Draw();
			}
		});
}

void RenderingSystem::RenderToFile()
{
	renderer_.renderTo(*offScreen_);
	DoRender();
	offScreen_->SaveToFile("out.png", 0, 0, width_, height_);
}

void RenderingSystem::RenderPickMap()
{
	renderer_.renderTo(*offScreen_);

	renderer_.setDepthTest(true);
	renderer_.setWireframeMode(false);
	renderer_.setFaceCulling(true);

	renderer_.setClearColor(glm::vec4(0.0f));
	renderer_.clearBuffers(Rendering::Renderer::ClearedBuffers::ColorDepth);
	renderer_.useShader(*pickShader_);

	pickShader_->SetUniform("camera", camera_.calculateTransformation());
	pickShader_->SetUniform("projection", projectionTrans_);

	// Pass - draw all
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

}

uint32_t RenderingSystem::QueryPickMap(int x, int y) const
{
	return offScreen_->QueryPixel(x, height_-y);
}


void RenderingSystem::SetViewport(int x, int y, int w, int h)
{
	renderer_.setViewport(x, y, w, h);
	projectionTrans_ = glm::perspective(glm::radians(45.0f),
			float(w)/h,
			0.1f, // near
			1000.0f // far
			);

	offScreenColor_ = std::make_unique<Rendering::Texture>();
	offScreenColor_->createEmpty(w, h);
	offScreen_->AttachColorBuffer(*offScreenColor_);
	offScreen_->attachDepthRenderbuffer(w, h);
	width_ = w;
	height_ = h;
}

void RenderingSystem::setCamera(const Rendering::Camera& cam)
{
	camera_ = cam;
}

