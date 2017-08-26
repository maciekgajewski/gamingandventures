set(CMAKE_CXX_FLAGS "-Wall -std=c++14")
project(balls1)
cmake_minimum_required(VERSION 3.0)


find_package(Boost 1.50)


include_directories(${Boost_INCLUDE_DIRS})

if(CMAKE_CL_64)
	if (NOT IRRLICH_ROOT)
		message(FATAL_ERROR "Set IRRLICH_ROOT to irrlicht distribution root")
	endif()
	include_directories(${IRRLICH_ROOT}/include)
	LINK_DIRECTORIES(${IRRLICH_ROOT}/lib/Win64-visualStudio)
endif()

add_executable(balls1
	main.cc
	ecs.cc ecs.hh
)

target_link_libraries(balls1

	Irrlicht)

