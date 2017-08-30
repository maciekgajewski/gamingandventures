add_library(glad STATIC

	glad.c glad.h
)

target_include_directories(glad PUBLIC ..)
target_link_libraries(glad
	dl
)
