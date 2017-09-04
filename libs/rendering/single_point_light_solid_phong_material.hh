#pragma once

#include <glm/glm.hpp>

#include "shader.hh"

namespace OT {

class SinglePointLightSolidPhongMaterial
{
public:
	SinglePointLightSolidPhongMaterial();

	void SetColor(const glm::vec3& color) { shader_.GetUniform("color").Set(color); }
	void SetLightPos(const glm::vec3& lightPos) { shader_.GetUniform("lightPos").Set(lightPos); }
	void SetLightColor(const glm::vec3& lightColor) { shader_.GetUniform("lightColor").Set(lightColor); }

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

