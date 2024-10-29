cmake_minimum_required(VERSION 3.28)
project(LangulusModPhysics
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for spatiality"
    HOMEPAGE_URL    https://langulus.com
)

file(GLOB_RECURSE
	LANGULUS_MOD_PHYSICS_SOURCES 
	LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
	source/*.cpp
)

# Build the module                                                              
add_langulus_mod(LangulusModPhysics ${LANGULUS_MOD_PHYSICS_SOURCES})

if(LANGULUS_TESTING)
    enable_testing()
	add_subdirectory(test)
endif()