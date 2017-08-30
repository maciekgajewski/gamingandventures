project(opengltest)
set(CMAKE_CXX_STANDARD 14)


add_executable(opengltest

	main.cc
)

target_link_libraries(opengltest
	glfw
)
