#include "single_point_light_phong_material.hh"

#include "io.hh"
#include "texture.hh"

namespace OT {

SinglePointLightPhongMaterial::SinglePointLightPhongMaterial()
	: shader_(readFile("shaders/single_light_phong.vert"), "single_light_phong.vert",
		readFile("shaders/single_light_phong.frag"), "single_light_phong.frag")
{
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
