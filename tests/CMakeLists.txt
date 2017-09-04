add_executable(ecs_test
	ecs_test.cc
)

target_link_libraries(ecs_test

	ecs
	gtest
	gtest_main
)
