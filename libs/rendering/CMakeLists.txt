add_library(rendering

	camera.cc camera.hh
	mesh.cc mesh.hh
	mesh_utilities.cc mesh_utilities.hh
	pvc_mesh.cc pvc_mesh.hh
	rendering_system.cc rendering_system.hh
	shader.cc shader.hh
	single_point_light_phong_material.cc single_point_light_phong_material.hh
	single_point_light_solid_phong_material.cc single_point_light_solid_phong_material.hh
	texture.cc texture.hh
	renderer.cc renderer.hh
	texture_manager.cc texture_manager.hh
	shader_manager.cc shader_manager.hh
)

target_include_directories(rendering INTERFACE ..)

target_link_libraries(rendering
	ecs

	glad
)

