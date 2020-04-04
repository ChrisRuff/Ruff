set( TARGET_TEST ${CMAKE_PROJECT_NAME}_test )
find_package(doctest REQUIRED)

file( GLOB_RECURSE TEST_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS *.cpp )

# only run if there are sources
list( LENGTH TEST_SRCS HAS_SOURCES )
if( ${HAS_SOURCES} GREATER 0 )

  add_executable( ${TARGET_TEST} ${TEST_SRCS} )

  # can use different options that the rest of the lib
  #colourized( "Test Compile Options: ${OPTS}" ${YLW} )
  target_compile_options( ${TARGET_TEST} PUBLIC ${OPTS} )

  set_target_properties( ${TARGET_TEST}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED True
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
  )

	target_link_libraries(${TARGET_TEST} doctest::doctest)
	target_link_libraries(${TARGET_TEST} Ruff)
	add_test( NAME ${TARGET_TEST} COMMAND ${TARGET_TEST} )
endif()

target_include_directories( ${TARGET_TEST} PUBLIC ${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
	${PROJECT_SOURCE_DIR}/src 
	/usr/include/doctest)

include(GNUInstallDirs)
