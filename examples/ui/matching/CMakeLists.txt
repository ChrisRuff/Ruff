cmake_minimum_required(VERSION 3.10)

file( GLOB_RECURSE SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS *.cpp )

# headers won't affect library but will let IDEs find them
file( GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS
	"*.h" "*.hpp")

# only run if source files were found
list( LENGTH SRCS HAS_SOURCES )
if( ${HAS_SOURCES} GREATER 0 )
	add_executable( matching ${HEADERS} ${SRCS})
	set_target_properties( matching
		PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED True
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
	)
	target_link_libraries(matching Ruff)

else()
	message(WARNING "No sources found")
endif()
