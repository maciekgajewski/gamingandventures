project(HordeTest)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(x_horde)

add_executable(hordeTest

	hordeTest.cc
)

target_include_directories(hordeTest PRIVATE include)

target_link_libraries(hordeTest
	Horde3D
	Horde3DUtils
	glfw.so.3
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
FIND_LIBRARY(X11_LIBRARY X11)
target_link_libraries(hordeTest ${X11_LIBRARY})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
