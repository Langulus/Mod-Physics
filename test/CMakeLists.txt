file(GLOB_RECURSE
	LANGULUS_MOD_PHYSICS_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp *.hpp
)

add_executable(LangulusModPhysicsTest ${LANGULUS_MOD_PHYSICS_TEST_SOURCES})

target_link_libraries(LangulusModPhysicsTest
	PRIVATE		Langulus
				Catch2
)

add_test(
	NAME		LangulusModPhysicsTest
	COMMAND		LangulusModPhysicsTest
)

langulus_copy_dlls(LangulusModPhysicsTest ON LangulusModPhysicsTest)
langulus_copy_dlls_advanced(LangulusModPhysicsTest
	TO			LangulusModPhysicsTest
	FROM		LangulusModPhysics
)