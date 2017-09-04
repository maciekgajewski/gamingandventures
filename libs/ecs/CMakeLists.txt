add_library(ecs

	ecs.cc ecs.hh
	visitor.cc visitor.hh
	component_type.cc component_type.hh
)

target_include_directories(ecs INTERFACE ..)
