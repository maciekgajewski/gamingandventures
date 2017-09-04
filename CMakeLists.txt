project(gamingadventures)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

FIND_PACKAGE(OpenGL REQUIRED)

# externals
# glad - code genrated and downloaded
add_subdirectory(glad)

# GLM - the maths library
set(GLM_STATIC_LIBRARY_ENABLE 1)
add_subdirectory(x_glm/glm glm)
include_directories(x_glm)

# STB - single file libs, header-only
include_directories(x_stb)

# Google test
add_subdirectory(x_googletest googletest)

# libraries

# apps
add_subdirectory(opengl2)

