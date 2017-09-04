// (C) 2017 Maciej Gajewski
#pragma once

#include "texture.hh"
#include "mesh.hh"
#include "single_point_light_phong_material.hh"
#include "camera.hh"

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

	OT::Mesh& Get(MeshId id) { return meshes_[id].mesh; }

private:

	struct LoadedMesh
	{
		OT::Mesh mesh;
		std::string path;
	};

	std::unordered_map<MeshId, LoadedMesh> meshes_;
};

class RenderingSystem
{
public:
	RenderingSystem();


	void Init(Ecs::Ecs& database);
	void Render(Ecs::Ecs& database);

	// Global settings
	void SetCamera(const Camera& cam);
	void SetCameraAspectRatio(float aspectRatio);
	void SetAmbientLight(const glm::vec3& color) { ambientLight_ = color; }
	void SetPointLight(const glm::vec3& position, const glm::vec3& color)
	{
		pointLightPos_ = position;
		pointLightColor_ = color;
	}

	void SetClearColor(const glm::vec3& color) { clearColor_ = color; }

	TextureStore textureStore;
	MeshStore meshStore;

private:

	Camera camera_;
	glm::mat4 projectionTrans_;

	glm::vec3 clearColor_ = glm::vec3(0.0f);

	// our poor lightining system
	glm::vec3 ambientLight_;
	glm::vec3 pointLightPos_;
	glm::vec3 pointLightColor_;

	OT::SinglePointLightPhongMaterial material_;
};

