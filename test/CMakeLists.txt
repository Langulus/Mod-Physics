file(GLOB_RECURSE
	LANGULUS_MOD_PHYSICS_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_executable(LangulusModPhysicsTest ${LANGULUS_MOD_PHYSICS_TEST_SOURCES})

target_link_libraries(LangulusModPhysicsTest
	PRIVATE		Langulus
				Catch2
)

add_dependencies(LangulusModPhysicsTest
	LangulusModPhysics
)

add_test(
	NAME		LangulusModPhysicsTest
	COMMAND		LangulusModPhysicsTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)