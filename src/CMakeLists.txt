# glob collection of src files not 100% reliable
file( GLOB_RECURSE SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS *.cpp )

# headers won't affect library but will let IDEs find them
file( GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS
	"*.hpp" "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp" )

# only run if source files were found
list( LENGTH SRCS HAS_SOURCES )
if( ${HAS_SOURCES} GREATER 0 )
	add_library( Ruff STATIC ${HEADERS} ${SRCS})

	set_target_properties( Ruff
		PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED True
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
	)

	# PRIVATE will not propogate opts to all linked targets
	target_compile_options( Ruff PRIVATE ${OPTS} )

	target_include_directories( Ruff PUBLIC 
		${PROJECT_BINARY_DIR} 
		${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
		${PROJECT_SOURCE_DIR}/src)

	# Make conan includes system dirs to prevent gcc warnings
	target_include_directories( Ruff SYSTEM PRIVATE 
		${CONAN_INCLUDE_DIRS_SDL2}
		${CONAN_INCLUDE_DIRS_SDL2_TTF}
		${CONAN_INCLUDE_DIRS_XTENSOR}
		${CONAN_INCLUDE_DIRS_OPENCV})

	target_link_libraries(Ruff PRIVATE
			CONAN_PKG::opencv
			CONAN_PKG::xtensor
			CONAN_PKG::sdl2
			CONAN_PKG::sdl2_ttf)
endif()

