#pragma once

#include <glm/glm.hpp>

#include "shader.hh"


namespace Rendering { class Texture;}

namespace OT {


class SinglePointLightPhongMaterial
{
public:
	SinglePointLightPhongMaterial();

	void Init();

	void SetLightPos(const glm::vec3& lightPos) { shader_.GetUniform("lightPos").Set(lightPos); }
	void SetLightColor(const glm::vec3& lightColor) { shader_.GetUniform("lightColor").Set(lightColor); }
	void SetDiffuseTexture(const Rendering::Texture& tex);

	void SetAmbientLight(const glm::vec3& ambientLight) { shader_.GetUniform("ambientLight").Set(ambientLight); }

	void SetLightAttenuation(const glm::vec3& attenuation) // const, linear, quadratic
	{
		shader_.GetUniform("attenuation").Set(attenuation);
	}

	void Use();

	Rendering::Shader& GetShader() { return shader_; }

private:

	Rendering::Shader shader_;
};

} // namespace OT

