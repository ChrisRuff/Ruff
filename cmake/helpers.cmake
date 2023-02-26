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

function(compile_flatbuffer FB_FILES)
	colourized("Compiling flatbuffer files:" ${BLU})
	string(REPLACE ";" "\n" fs "${FB_FILES}")
	colourized("${fs}\n" ${BLU})

	foreach(F ${FB_FILES})
		get_filename_component(OUT_DIR ${F} DIRECTORY)
		execute_process(
				COMMAND
					flatc -c --scoped-enums --gen-object-api --gen-mutable
				          --filename-suffix _buf --filename-ext hpp
						  --cpp-std c++17 --cpp-static-reflection --reflect-names
						  ${F}
				WORKING_DIRECTORY
					${OUT_DIR})
	endforeach()
endfunction()

function(get_files COMPONENT)
	file( GLOB_RECURSE FB_${COMPONENT} LIST_DIRECTORIES false CONFIGURE_DEPENDS
			"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${COMPONENT}/*.fbs" )
	#list(LENGTH ${FB_${COMPONENT}} FB_SIZE)
	if(FB_${COMPONENT})
		compile_flatbuffer(${FB_${COMPONENT}})
	endif()

	file( GLOB_RECURSE SRC_${COMPONENT} LIST_DIRECTORIES false CONFIGURE_DEPENDS
			"${PROJECT_SOURCE_DIR}/src/${COMPONENT}/*.cpp" )
	set(SRC_${COMPONENT} ${SRC_${COMPONENT}} PARENT_SCOPE)

	# headers won't affect library but will let IDEs find them
	file( GLOB_RECURSE HEADERS_${COMPONENT} LIST_DIRECTORIES false CONFIGURE_DEPENDS
			"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${COMPONENT}/*.hpp"
			"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${COMPONENT}/*.tpp") # templated implementation files
	set(HEADERS_${COMPONENT} ${HEADERS_${COMPONENT}} PARENT_SCOPE)

endfunction()


function(make_component COMPONENT)
	colourized("TARGET ${COMPONENT}" ${BLU})
	get_files(${COMPONENT} HEADERS SRC)
	string(REPLACE ";" "\n" hs "${HEADERS_${COMPONENT}}")
	colourized("HEADERS:\n${hs}" ${CYN})

	string(REPLACE ";" "\n" ss "${SRC_${COMPONENT}}")
	colourized("SRC:\n${ss}\n" ${CYN})

	add_library( ${COMPONENT} STATIC ${HEADERS_${COMPONENT}} ${SRC_${COMPONENT}})
	set_target_properties( ${COMPONENT} PROPERTIES OUTPUT_NAME "ruff-${COMPONENT}")
	add_library( ruff::${COMPONENT} ALIAS ${COMPONENT})
	set_target_properties( ${COMPONENT}
		PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED True
		LINKER_LANGUAGE CXX
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
	)
	target_compile_options( ${COMPONENT} PRIVATE ${OPTS} )

	target_include_directories( ${COMPONENT} PUBLIC 
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

	if(OPENMP_FOUND)
		target_link_libraries( ${COMPONENT} PUBLIC OpenMP::OpenMP_CXX)
	endif()

	install(TARGETS ${COMPONENT}
					EXPORT  ${COMPONENT}-targets
					COMPONENT ${COMPONENT}
					LIBRARY DESTINATION lib
					ARCHIVE DESTINATION lib
					RUNTIME DESTINATION bin
					INCLUDES DESTINATION include/ruff)

	install(EXPORT ${COMPONENT}-targets
					FILE   "${COMPONENT}-targets.cmake"
					NAMESPACE ${CMAKE_PROJECT_NAME}::
					DESTINATION lib/cmake/${CMAKE_PROJECT_NAME}
					COMPONENT ${COMPONENT})

	# copy config file to build tree
	configure_file("${PROJECT_SOURCE_DIR}/cmake/${COMPONENT}-config.cmake.in"
					"${PROJECT_BINARY_DIR}/${COMPONENT}-config.cmake"
					@ONLY)

	# add component's config file to installation
	install(FILES "${PROJECT_BINARY_DIR}/${COMPONENT}-config.cmake"
					DESTINATION "lib/cmake/${CMAKE_PROJECT_NAME}"
					COMPONENT ${COMPONENT})

	# add component's public headers to installation
	install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/ruff/${COMPONENT}"
					COMPONENT ${COMPONENT}
					DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ruff")

endfunction()

function(make_example COMPONENT)
	file( GLOB_RECURSE SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
			"${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/*.c"
			"${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/*.cpp")
	# headers won't affect library but will let IDEs find them
	file( GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS
			${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/"*.h"
			${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/"*.hpp")

	colourized("TARGET ${COMPONENT}" ${BLU})
	string(REPLACE ";" "\n" hs "${HEADERS}")
	colourized("HEADERS:\n${hs}" ${CYN})

	string(REPLACE ";" "\n" ss "${SRCS}")
	colourized("SRC:\n${ss}\n" ${CYN})

	add_executable( ${COMPONENT} ${HEADERS} ${SRCS})
	set_target_properties( ${COMPONENT}
			PROPERTIES
			CXX_STANDARD 20
			CXX_STANDARD_REQUIRED True
			ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
			LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
			RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
			)
	target_link_libraries(${COMPONENT} ruff::core)

	target_compile_options( ${COMPONENT} PRIVATE ${OPTS} )
	target_include_directories( ${COMPONENT} PUBLIC
			$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}>
			$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:include>
			PRIVATE
			${PROJECT_BINARY_DIR})

endfunction()
