cmake_minimum_required(VERSION 3.10)

find_package(xtensor REQUIRED)
find_package(OpenCV REQUIRED)

# glob collection of src files not 100% reliable
file( GLOB_RECURSE SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS *.cpp )

# headers won't affect library but will let IDEs find them
file( GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS
	"*.hpp" "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp" )

# only run if source files were found
list( LENGTH SRCS HAS_SOURCES )
if( ${HAS_SOURCES} GREATER 0 )
  add_library( Ruff STATIC ${HEADERS} ${SRCS} )

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
		${OpenCV_INCLUDE_DIRS})
endif()

# Build CScope definitions
add_custom_command(TARGET Ruff POST_BUILD COMMAND find ${PROJECT_SOURCE_DIR} -name "*.cpp" -o -name "*.hpp" > ${PROJECT_SOURCE_DIR}/cscope.files)
message(WARNING "${PROJECT_SOURCE_DIR}")
add_custom_command(TARGET Ruff POST_BUILD COMMAND cscope -Rbkq ${PROJECT_SOURCE_DIR}/cscope.files)
add_custom_command(TARGET Ruff POST_BUILD COMMAND bash ${PROJECT_SOURCE_DIR}/Ruff_env.cscope)
target_link_libraries(Ruff xtensor)
target_link_libraries(Ruff ${OpenCV_LIBRARIES})
