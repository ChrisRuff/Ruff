set( TARGET_TEST ${CMAKE_PROJECT_NAME}_test )

file( GLOB_RECURSE TEST_SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS *.cpp )
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

target_link_libraries(${TARGET_TEST} CONAN_PKG::doctest)
target_link_libraries(${TARGET_TEST}
		ruff::core
		ruff::geometry
		ruff::ui
		ruff::nlp
		ruff::ai
		ruff::imgproc
		ruff::security
		ruff::photogrammetry)

foreach(component core geometry ui nlp ai security imgproc photogrammetry)
	#file( GLOB_RECURSE FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${component}/*.cpp)
	add_test(NAME ${component}_test COMMAND ${TARGET_TEST} -sf=${CMAKE_CURRENT_LIST_DIR}/${component}/*.cpp)
endforeach()

include(CodeCoverage)

APPEND_COVERAGE_COMPILER_FLAGS()
SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(
		NAME coverage
		EXECUTABLE ${TARGET_TEST})

include(GNUInstallDirs)
