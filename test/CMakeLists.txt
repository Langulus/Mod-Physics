file(GLOB_RECURSE
	LANGULUS_MOD_PHYSICS_TEST_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	*.cpp
)

add_langulus_test(LangulusModPhysicsTest
	SOURCES			${LANGULUS_MOD_PHYSICS_TEST_SOURCES}
	LIBRARIES		Langulus
	DEPENDENCIES    LangulusModPhysics
)