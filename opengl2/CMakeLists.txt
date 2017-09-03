project(opengltest2)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

FIND_PACKAGE(OpenGL REQUIRED)

add_subdirectory(ecs)
add_subdirectory(tests)

# externals

# glad - code genrated and downloaded
add_subdirectory(glad)

# GLM - the maths library
set(GLM_STATIC_LIBRARY_ENABLE 1)
add_subdirectory(../x_glm/glm glm)
include_directories(../x_glm)

# STB - single file libs, header-only
include_directories(../x_stb)

# Google test
add_subdirectory(../x_googletest googletest)

file(GLOB SHADERS shaders/*)
add_executable(opengltest2

	main.cc
	window.hh window.cc
	io.hh io.cc

	shader.hh shader.cc
	mesh.hh mesh.cc
	pvc_mesh.hh pvc_mesh.cc
	mesh_utilities.hh mesh_utilities.cc
	single_point_light_solid_phong_material.hh single_point_light_solid_phong_material.cc
	single_point_light_phong_material.hh single_point_light_phong_material.cc
	texture.hh texture.cc

	${SHADERS}
)

target_link_libraries(opengltest2
	glfw
	glad
	glm_static
	${OpenGL_LIBRARIES}
)
