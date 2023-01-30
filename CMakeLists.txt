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

include(CMakeDependentOption)
option(BUILD_TESTERS "Build testers" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DOXY_DOC "Build Doxygen documentation" ON)

option(BUILD_GEOMETRY "Build geometry" ON)
cmake_dependent_option(BUILD_UI "Build ui engine" ON "BUILD_GEOMETRY" ON "BUILD_IMGPROC" ON)
option(BUILD_AI "Build ai module" ON)
option(BUILD_SECURITY "Build security module" ON)
option(BUILD_NLP "Build natural language processing module" ON)
option(BUILD_IMGPROC "Build image processing module" ON)
cmake_dependent_option(BUILD_PHOTOGRAMMETRY "Build photogrammetry module" ON "BUILD_IMGPROC" ON)

cmake_dependent_option(USE_SDL2 "Use SDL2 for screen" ON "BUILD_UI" ON)
cmake_dependent_option(USE_EMSCRIPTEN "Generate Emscripten files" OFF "BUILD_UI" ON)

option(SILENT_CONAN "Suppress conan messages" ON)
if(SILENT_CONAN)
	set(CONAN_CMAKE_SILENT_OUTPUT ON)
endif()
include("${PROJECT_SOURCE_DIR}/cmake/Conan.cmake")

if(USE_EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

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
					-Weffc++ -Woverloaded-virtual -Wsign-promo -march=native -fmax-errors=5)
endif()

colourized( "Compile Options: ${OPTS}" ${YLW} )

# Add deps
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
set(CMAKE_BUILD_TYPE "Debug")

add_compile_definitions(DATA_DIR="${PROJECT_SOURCE_DIR}/data")
# Add target Ruff
colourized( "Configuring Ruff Library" ${BLU} )
add_subdirectory(src)

if(BUILD_DOXY_DOC)
	find_package(Doxygen REQUIRED)
	colourized( "Configuring Docs" ${BLU} )
	add_subdirectory(docs)
endif()

if(BUILD_TESTERS)
	colourized( "Configuring Testers Library" ${BLU} )
	find_package(doctest REQUIRED)
	enable_testing()

	add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
	colourized( "Configuring examples" ${BLU} )
	add_subdirectory(examples)
endif()

configure_file( 
	${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake.in
	${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake
	@ONLY
)

install(FILES
	"${PROJECT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake"
		DESTINATION lib/cmake/${CMAKE_PROJECT_NAME})
