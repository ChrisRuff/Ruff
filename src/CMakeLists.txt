cmake_minimum_required(VERSION 3.10)

find_package(xtensor REQUIRED)
find_package(OpenCV REQUIRED PATH_SUFFIXES "*.a")
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL2TTF REQUIRED)

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
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED True
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
	)

	# PUBLIC will propogate opts to all linked targets
	target_compile_options( Ruff PUBLIC ${OPTS} )

	target_include_directories( Ruff PUBLIC 
		${PROJECT_BINARY_DIR} 
		${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
		${PROJECT_SOURCE_DIR}/src 
		${PROJECT_SOURCE_DIR}/3rdparty
		${SDL_INCLUDE_DIR}
		${OpenCV_INCLUDE_DIR})

	target_link_libraries(Ruff xtensor)
	target_link_libraries(Ruff ${OpenCV_LIBRARIES})
	target_link_libraries(Ruff ${X11_LIBRARIES})
	target_link_libraries(Ruff ${OPENGL_LIBRARY})
	target_link_libraries(Ruff SDL2 ${SDL2TTF_LIBRARY})

	#target_link_libraries(Ruff 3rdParty)
else()
	message(WARNING "No sources found")
endif()
