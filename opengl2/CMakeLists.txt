
file(GLOB SHADERS ../shaders/*)
add_executable(opengltest2

	main.cc

	main_window.cc main_window.hh
	rendering_system.cc rendering_system.hh
	input_system.cc input_system.hh

	${SHADERS}
)

target_link_libraries(opengltest2
	ecs
	rendering
	utils
	glm_aabb

	glm_static
	${OpenGL_LIBRARIES}
)
