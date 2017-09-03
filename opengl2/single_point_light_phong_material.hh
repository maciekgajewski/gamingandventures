#pragma once

#include <glm/glm.hpp>

#include "shader.hh"

namespace OT {

class Texture;

class SinglePointLightPhongMaterial
{
public:
	SinglePointLightPhongMaterial();

	void SetLightPos(const glm::vec3& lightPos) { shader_.GetUniform("lightPos").Set(lightPos); }
	void SetLightColor(const glm::vec3& lightColor) { shader_.GetUniform("lightColor").Set(lightColor); }
	void SetDiffuseTexture(const Texture& tex);

	void SetAmbientLight(const glm::vec3& ambientLight) { shader_.GetUniform("ambientLight").Set(ambientLight); }

	void SetLightAttenuation(const glm::vec3& attenuation) // const, linear, quadratic
	{
		shader_.GetUniform("attenuation").Set(attenuation);
	}

	void Use();

	Shader& GetShader() { return shader_; }

private:

	Shader shader_;
};

} // namespace OT

