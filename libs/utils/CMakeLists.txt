add_library(utils
	io.cc io.hh
	window.cc window.hh
)

target_include_directories(utils INTERFACE ..)

target_link_libraries(utils

	glad
	glfw
)
