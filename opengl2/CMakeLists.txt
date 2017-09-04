
file(GLOB SHADERS ../shaders/*)
add_executable(opengltest2

	main.cc

	main_window.cc main_window.hh

	${SHADERS}
)

target_link_libraries(opengltest2
	ecs
	rendering
	utils

	glm_static
	${OpenGL_LIBRARIES}
)
