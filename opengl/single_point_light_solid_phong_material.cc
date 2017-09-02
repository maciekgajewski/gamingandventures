#include "single_point_light_solid_phong_material.hh"

#include "io.hh"

namespace OT {

SinglePointLightSolidPhongMaterial::SinglePointLightSolidPhongMaterial()
{
	shader_ = Shader(
		readFile("shaders/single_light_solid_phong.vert"),
		readFile("shaders/single_light_solid_phong.frag"));

}

void SinglePointLightSolidPhongMaterial::Use()
{
	shader_.Use();
}

} // namespace OT
