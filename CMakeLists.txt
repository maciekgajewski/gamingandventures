project(gamingadventures)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

FIND_PACKAGE(OpenGL REQUIRED)
find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)

include_directories(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

if(GLFW_ROOT)
	include_directories(${GLFW_ROOT}/include)
	LINK_DIRECTORIES(${GLFW_ROOT}/lib)
endif()

include_directories(${ASSIMP_INCLUDE_DIRS})
link_directories(${ASSIMP_LIBRARY_DIRS})

# externals
# glad - code genrated and downloaded
add_subdirectory(glad)

# GLM - the maths library
set(GLM_STATIC_LIBRARY_ENABLE 1)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(x_glm/glm glm)
include_directories(x_glm)

# AABB extension to glm
add_subdirectory(x_glm_aabb)
include_directories(x_glm_aabb)

# STB - single file libs, header-only
include_directories(x_stb)

# Google test
add_subdirectory(x_googletest googletest)

# Ass Imp
add_subdirectory(x_assimp)
include_directories(x_assimp/include)
include_directories(${CMAKE_BINARY_DIR}/x_assimp/include/)

# libraries
add_subdirectory(libs)

# apps
add_subdirectory(opengl2)

# tests
add_subdirectory(tests)

