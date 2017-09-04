#include "single_point_light_phong_material.hh"

#include <utils/io.hh>

#include "texture.hh"

namespace OT {

SinglePointLightPhongMaterial::SinglePointLightPhongMaterial()
	: shader_()
{
}

void SinglePointLightPhongMaterial::Init()
{
	shader_ = OT::Shader(
		readFile("shaders/single_light_phong.vert"), "single_light_phong.vert",
		readFile("shaders/single_light_phong.frag"), "single_light_phong.frag");
}

void SinglePointLightPhongMaterial::SetDiffuseTexture(const Texture& tex)
{
	tex.Activate(0);
}

void SinglePointLightPhongMaterial::Use()
{
	shader_.Use();
}

} // namespace OT
