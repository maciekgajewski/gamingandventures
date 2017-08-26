set(CMAKE_CXX_FLAGS "-Wall -std=c++14")
project(balls1)
cmake_minimum_required(VERSION 3.0)

find_package(Boost 1.50)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(balls1
	main.cc
	ecs.cc ecs.hh
)

target_link_libraries(balls1

	Irrlicht)

