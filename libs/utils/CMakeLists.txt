add_library(utils
	io.cc io.hh
	window.cc window.hh
)

target_include_directories(utils INTERFACE ..)

if(WIN32)
	set(GLFW3_LIB glfw3)
else()
	set(GLFW3_LIB glfw)
endif()

target_link_libraries(utils

	glad
	${GLFW3_LIB}
)

