// (C) 2017 Maciej Gajewski
#include "rendering_system.hh"

#include <ecs/ecs.hh>

#include <algorithm>

RenderingSystem::RenderingSystem()
{

}

void RenderingSystem::Init(Ecs::Ecs& database)
{
	database.RegisterUniqueComponentType<Components::Material>("Material");
	database.RegisterUniqueComponentType<Components::Mesh>("Mesh");
	database.RegisterUniqueComponentType<Components::Transformation>("Transformation");
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
