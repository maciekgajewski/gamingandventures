add_library(rendering

	camera.cc camera.hh
	mesh.cc mesh.hh
	mesh_utilities.cc mesh_utilities.hh
	pvc_mesh.cc pvc_mesh.hh
	shader.cc shader.hh
	texture.cc texture.hh
	renderer.cc renderer.hh
	texture_manager.cc texture_manager.hh
	shader_manager.cc shader_manager.hh
	mesh_manager.cc mesh_manager.hh
	components.cc components.hh
	framebuffer.cc framebuffer.hh
)

target_include_directories(rendering INTERFACE ..)

target_link_libraries(rendering
	ecs

	glad
	assimp
)

