cmake_minimum_required(VERSION 3.16.3)

project( ruff VERSION 0.0.1
								LANGUAGES CXX
								DESCRIPTION "Ruff")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/helpers.cmake)
colourized("CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}" ${MAG} )
colourized("PROJECT_NAME: ${PROJECT_BINARY_DIR}" ${MAG} )
colourized("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}" ${MAG} )
colourized("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}" ${MAG} )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTERS "Build testers" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_DOXY_DOC "Build Doxygen documentation" OFF)

# append path to search for cmake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# support IDE folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# turn off compiler specific extensions
set( CMAKE_CXX_EXTENSIONS OFF )

include( cmake/CppCheck.cmake )
include( cmake/ClangFormat.cmake )
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options (-fdiagnostics-color=always)
	set(OPTS -Wall -Wextra -Wctor-dtor-privacy -Wno-init-list-lifetime
					-Wnoexcept -Wstrict-null-sentinel -Wold-style-cast 
					-Weffc++ -Woverloaded-virtual -Wsign-promo -march=native)
endif()

colourized( "Compile Options: ${OPTS}" ${YLW} )


# Add deps
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

# Add target Ruff
colourized( "Configuring Ruff Library" ${BLU} )
add_subdirectory(src)

if(BUILD_DOXY_DOC)
	find_package(Doxygen REQUIRED)
	colourized( "Configuring Docs" ${BLU} )
	add_subdirectory(docs)
endif()

if(BUILD_TESTERS)
	find_package(doctest REQUIRED)
	colourized( "Configuring Testers Library" ${BLU} )
	add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
	colourized( "Configuring examples" ${BLU} )
	add_subdirectory(examples)
	file(GLOB_RECURSE RESOURCES 
		LIST_DIRECTORIES false 
		CONFIGURE_DEPENDS 
		${PROJECT_SOURCE_DIR}/examples/*.bmp
		${PROJECT_SOURCE_DIR}/examples/*.png
		${PROJECT_SOURCE_DIR}/examples/*.ttf
		${PROJECT_SOURCE_DIR}/examples/*.obj
		${PROJECT_SOURCE_DIR}/test/*.png)
	file(COPY ${RESOURCES} DESTINATION "${PROJECT_SOURCE_DIR}/bin")
endif()

configure_file( 
	${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake.in
	${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake
	@ONLY
)

install(FILES
	"${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake"
		DESTINATION lib/cmake/${CMAKE_PROJECT_NAME})
