// (C) 2017 Maciej Gajewski
#include "rendering_system.hh"

#include <ecs/ecs.hh>
#include <ecs/visitor.hh>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

RenderingSystem::RenderingSystem()
{
	ambientLight_ = glm::vec3(1.0f);
	pointLightPos_ = glm::vec3(0.0f);
	pointLightColor_ = glm::vec3(0.0f);
}

void RenderingSystem::Init(Ecs::Ecs& database)
{
	database.RegisterUniqueComponentType<Components::Material>("Material");
	database.RegisterUniqueComponentType<Components::Mesh>("Mesh");
	database.RegisterUniqueComponentType<Components::Transformation>("Transformation");

	material_.Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderingSystem::Render(Ecs::Ecs& database)
{
	glClearColor(clearColor_.r, clearColor_.g, clearColor_.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	material_.Use();
	material_.SetAmbientLight(ambientLight_);
	material_.SetLightColor(pointLightColor_);
	material_.SetLightPos(pointLightPos_);

	material_.GetShader().GetUniform("camera").Set(camera_.CalculateTransformation());
	material_.GetShader().GetUniform("projection").Set(projectionTrans_);

	OT::Uniform modelUniform = material_.GetShader().GetUniform("model");

	// Iterate
	auto visitor = Ecs::BuildUniqueTypeVisitor<Components::Material, Components::Mesh, Components::Transformation>(database);

	visitor.ForEach([&](
		Ecs::EntityId id,
		const Components::Material& material,
		const Components::Mesh& mesh,
		const Components::Transformation& trans)
		{
			modelUniform.Set(trans.transformation);

			OT::Texture& texture = textureStore.GetTexture(material.diffuseTextureId);
			material_.SetDiffuseTexture(texture);

			OT::Mesh& m = meshStore.Get(mesh.meshId);
			m.Draw();
		});

}

void RenderingSystem::SetCamera(const Camera& cam)
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

TextureId TextureStore::LoadTexture(const std::string& path)
{
	auto it = std::find_if(textures_.begin(), textures_.end(),
		[&](auto& kv)
		{
			return kv.second.path == path;
		});

	TextureId id;

	if (it == textures_.end())
	{
		LoadedTexture t;
		t.texture.LoadFromFile(path, OT::Texture::GENERATE_MIPMAPS);
		t.path = path;
		id = textures_.size() + 1;

		textures_.emplace(id, std::move(t));
	}
	else
	{
		id = it->first;
	}

	return id;
}

MeshId MeshStore::GetOrGenerate(const std::string& name, std::function<OT::Mesh ()> generator)
{
	auto it = std::find_if(meshes_.begin(), meshes_.end(),
		[&](auto& kv)
		{
			return kv.second.path == name;
		});

	MeshId id;

	if (it == meshes_.end())
	{
		LoadedMesh m;

		m.mesh = generator();
		m.path = name;

		id = meshes_.size() + 1;

		meshes_.emplace(id, std::move(m));
	}
	else
	{
		id = it->first;
	}

	return id;
}
