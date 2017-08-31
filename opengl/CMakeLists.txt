project(opengltest)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

FIND_PACKAGE(OpenGL REQUIRED)

# externals
add_subdirectory(glad)

set(GLM_STATIC_LIBRARY_ENABLE 1)
add_subdirectory(x_glm/glm)
include_directories(x_glm)


add_executable(opengltest

	main.cc
	window.hh window.cc
	io.hh io.cc

	shader.hh shader.cc

	shader.vert
	shader.frag
)

target_link_libraries(opengltest
	glfw
	glad
	glm_static
	${OpenGL_LIBRARIES}
)
