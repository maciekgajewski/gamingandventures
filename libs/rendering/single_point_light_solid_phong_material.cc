#include "single_point_light_solid_phong_material.hh"

#include <utils/io.hh>

namespace OT {

SinglePointLightSolidPhongMaterial::SinglePointLightSolidPhongMaterial()
	: shader_(readFile("shaders/single_light_solid_phong.vert"), "single_light_solid_phong.vert",
		readFile("shaders/single_light_solid_phong.frag"), "single_light_solid_phong.frag")
{
}

void SinglePointLightSolidPhongMaterial::Use()
{
	shader_.Use();
}

} // namespace OT
