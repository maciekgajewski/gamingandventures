add_subdirectory(ecs)
add_subdirectory(tests)

file(GLOB SHADERS ../shaders/*)
add_executable(opengltest2

	main.cc

	main_window.cc main_window.hh
	rendering_system.cc rendering_system.hh


	window.hh window.cc
	io.hh io.cc

	shader.hh shader.cc
	mesh.hh mesh.cc
	pvc_mesh.hh pvc_mesh.cc
	mesh_utilities.hh mesh_utilities.cc
	single_point_light_solid_phong_material.hh single_point_light_solid_phong_material.cc
	single_point_light_phong_material.hh single_point_light_phong_material.cc
	texture.hh texture.cc
	camera.hh camera.cc

	${SHADERS}
)

target_link_libraries(opengltest2
	ecs
	glfw
	glad
	glm_static
	${OpenGL_LIBRARIES}
)
