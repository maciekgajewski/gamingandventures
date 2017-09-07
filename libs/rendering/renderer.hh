// (C) 2017 Maciej Gajewski
#pragma once

#include "texture_manager.hh"
#include "shader_manager.hh"
#include "mesh_manager.hh"

#include <glm/glm.hpp>

namespace Rendering {

class Shader;
class Texture;

// Renderer is simple rendering API, which one can use to implement rendering pipeline.
//
// The goal of this class is not to wrap OpenGL, but rather to keep inventory of OpenGL functions used
class Renderer
{
public:
	Renderer();

	void SetClearColor(const glm::vec3& color);

	enum class ClearedBuffers { Color, Depth, ColorDepth };
	void ClearBuffers(ClearedBuffers b);

	void SetDepthTest(bool enabled);

	void SetWireframeMode(bool wireframe);

	void UseShader(Shader& shader);
	void ActivateTexture(Texture& unit);

	TextureManager& Textures() { return textureManager_; }
	ShaderManager& Shaders() { return shaderManager_; }
	MeshManager& Meshes() { return meshManager_; }

private:

	TextureManager textureManager_;
	ShaderManager shaderManager_;
	MeshManager meshManager_;

};

} // namespace Rendering

