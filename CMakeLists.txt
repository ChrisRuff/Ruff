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

cmake_minimum_required(VERSION 3.16.5)

project( Ruff VERSION 0.0.1 
            DESCRIPTION "Ruff"
            LANGUAGES CXX )

colourized("CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}" ${MAG} )
colourized("PROJECT_NAME: ${PROJECT_BINARY_DIR}" ${MAG} )
colourized("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}" ${MAG} )
colourized("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}" ${MAG} )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTERS "Build testers" OFF)

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

  find_package( Doxygen )
  if( Doxygen_FOUND )
    add_subdirectory( docs )
  else()
    message( STATUS "Doxygen not found, not building docs" )
  endif()

endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(OPTS -Wall -Wextra -Werror -Wctor-dtor-privacy -Wno-init-list-lifetime
            -Wnoexcept -Weffc++ -Wstrict-null-sentinel -Wold-style-cast 
            -Woverloaded-virtual -Wsign-promo)
endif()

colourized( "Compile Options: ${OPTS}" ${YLW} )

if(BUILD_TESTERS)
	add_definitions(-DBUILD_TESTERS)
endif()

add_subdirectory(src)

if(BUILD_TESTERS)
	add_subdirectory(test)
endif()

configure_file( 
  ${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.hpp.in
  ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}-config.hpp
)
