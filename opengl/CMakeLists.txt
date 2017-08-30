project(opengltest)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

FIND_PACKAGE(OpenGL REQUIRED)

add_subdirectory(glad)

add_executable(opengltest

	main.cc
	window.hh window.cc
	io.hh io.cc

	shader.vert
)

target_link_libraries(opengltest
	glfw
	glad
	${OpenGL_LIBRARIES}
)
