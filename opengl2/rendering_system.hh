// (C) 2017 Maciej Gajewski
#pragma once

#include "texture.hh"
#include "mesh.hh"

#include <glm/glm.hpp>

#include <cstdint>
#include <unordered_map>
#include <functional>

namespace Ecs { class Ecs; }

using TextureId = std::uint64_t;
using MeshId =std::uint64_t;

// rendering components
namespace Components
{

// Required for rendering
struct Material
{
	TextureId diffuseTextureId = 0;
	float shininess = 0.0f;
};

struct Mesh
{
	MeshId meshId = 0;
};

struct Transformation
{
	glm::mat4 transformation = glm::mat4(1.0f);
};

}

// Stores loaded textures
class TextureStore
{
public:

	TextureId LoadTexture(const std::string& path);

	OT::Texture& GetTexture(TextureId id)
	{
		return textures_[id].texture;
	}

private:

	struct LoadedTexture
	{
		std::string path;
		OT::Texture texture;
	};

	std::unordered_map<TextureId, LoadedTexture> textures_;
};

// Stores loaded/generated meshes
class MeshStore
{
public:

	MeshId GetOrGenerate(const std::string& name, std::function<OT::Mesh()> generator);

private:

	struct LoadedMesh
	{
		OT::Mesh mesh;
		std::string path;
	};

	std::unordered_map<TextureId, LoadedMesh> meshes_;
};

class RenderingSystem
{
public:
	RenderingSystem();


	void Init(Ecs::Ecs& database);
	void Render(Ecs::Ecs& database);

	// Global settings
	void SetCameraPosition(const glm::vec3& pos, const glm::vec3& look, const glm::vec3& up); // TODO make entity
	void SetAmbientLight(const glm::vec3& color);

	TextureStore textureStore;
	MeshStore meshStore;
};

