if( NOT WIN32 )
	set( BLU "[1;34m" CACHE STRING "AnsiBlue" )
	set( RED "[1;31m" CACHE STRING "AnsiRed" )
	set( MAG "[1;35m" CACHE STRING "AnsiMagenta" )
	set( CYN "[1;36m" CACHE STRING "AnsiCyan" )
	set( YLW "[1;33m" CACHE STRING "AnsiYellow" )
	set( WHT "[37m" CACHE STRING "AnsiWhite" )
	set( BLK "[30m" CACHE STRING "AnsiBlack" )
	set( RST "[0m" CACHE STRING "AnsiReset" )
	function( colourized MESSAGE COL )
		message( "${COL}${MESSAGE}${RST}" )
	endfunction()
endif()
cmake_minimum_required(VERSION 3.17)


project( ruff VERSION 0.0.1 
						DESCRIPTION "Ruff"
						LANGUAGES CXX )

include(cmake/Conan.cmake)
run_conan()

colourized("CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}" ${MAG} )
colourized("PROJECT_NAME: ${PROJECT_BINARY_DIR}" ${MAG} )
colourized("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}" ${MAG} )
colourized("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}" ${MAG} )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Release")

option(BUILD_TESTERS "Build testers" ON)
option(BUILD_TESTERS "Build examples" ON)

include (cmake/Cache.cmake)

if( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME )

	# append path to search for cmake modules
	set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )

	# support IDE folders
	set_property( GLOBAL PROPERTY USE_FOLDERS ON )

	# turn off compiler specific extensions
	set( CMAKE_CXX_EXTENSIONS OFF )

	# must be done in main CMakeLists
	# enables testing and provides BUILD_TESTING option
	include( CTest )

	include( cmake/CppCheck.cmake )
	include( cmake/ClangFormat.cmake )

endif()


if(BUILD_TESTERS)
	add_definitions(-DBUILD_TESTERS)
endif()


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options (-fdiagnostics-color=always)
	set(OPTS -Wall -Wextra -Wctor-dtor-privacy -Wno-init-list-lifetime
					-Wnoexcept -Wstrict-null-sentinel -Wold-style-cast 
					-Weffc++ -Woverloaded-virtual -Wsign-promo)
endif()

colourized( "Compile Options: ${OPTS}" ${YLW} )

find_package( Doxygen )
if( Doxygen_FOUND )
	colourized( "Configuring Docs" ${BLU} )
	add_subdirectory( docs )
else()
	message( STATUS "Doxygen not found, not building docs" )
endif()

# Add target Ruff
colourized( "Configuring Ruff Library" ${BLU} )
add_subdirectory(src)


if(BUILD_TESTERS)
	colourized( "Configuring Testers Library" ${BLU} )
	add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
	colourized( "Configuring examples" ${BLU} )
	add_subdirectory(examples)
endif()

configure_file( 
	${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.hpp.in
	${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}-config.hpp
)
