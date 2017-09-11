// (C) 2017 Maciej Gajewski
#pragma once

#include "texture_manager.hh"
#include "shader_manager.hh"
#include "mesh_manager.hh"

#include <glm/glm.hpp>

namespace Rendering {

class Shader;
class Texture;
class Framebuffer;

// Renderer is simple rendering API, which one can use to implement rendering pipeline.
//
// The goal of this class is not to wrap OpenGL, but rather to keep inventory of OpenGL functions used
class Renderer
{
public:
	Renderer();

	// state
	void setClearColor(const glm::vec4& color);
	void setViewport(int x, int y, int w, int h);

	enum class ClearedBuffers { Color, Depth, ColorDepth };
	void clearBuffers(ClearedBuffers b);

	void setDepthTest(bool enabled); // enables/disbales depth test
	void setDepthMask(bool enabled); // enables/disbales writing to depth buffer
	void setFaceCulling(bool enabled);
	void setWireframeMode(bool wireframe);
	void setBlend(bool enabled); // enables/disables alphablending

	void useShader(Shader& shader);
	void activateTexture(Texture& texture, unsigned unit);
	void activateCubemap(Texture& texture);

	// render target
	void renderToScreen();
	void renderTo(Framebuffer& fb);


	// Resource managers
	TextureManager& textures() { return textureManager_; }
	ShaderManager& shaders() { return shaderManager_; }
	MeshManager& meshes() { return meshManager_; }

	static void checkError();

private:

	TextureManager textureManager_;
	ShaderManager shaderManager_;
	MeshManager meshManager_;

};

} // namespace Rendering

