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

function(make_example COMPONENT )
	file( GLOB_RECURSE SRCS LIST_DIRECTORIES false CONFIGURE_DEPENDS
		"${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/*.c" 
		"${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/*.cpp")
	# headers won't affect library but will let IDEs find them
	file( GLOB_RECURSE HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS
		${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/"*.h" 
		${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}/"*.hpp")

	add_executable( ${COMPONENT} ${HEADERS} ${SRCS})
	set_target_properties( ${COMPONENT}
		PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED True
		ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
	)
	target_link_libraries(${COMPONENT} ruff::core ruff::geometry ruff::ui)

	target_compile_options( ${COMPONENT} PRIVATE ${OPTS} )
	target_include_directories( ${COMPONENT} PUBLIC 
			$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}>
			$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
			$<INSTALL_INTERFACE:include>
			PRIVATE
			${PROJECT_BINARY_DIR})
	target_compile_definitions(${COMPONENT} PRIVATE -DDATA_DIR="${PROJECT_SOURCE_DIR}/data")
endfunction()

function(make_component COMPONENT HEADERS SRC)
	colourized("TARGET ${COMPONENT}\n" ${BLU})
	string(REPLACE ";" "\n" hs "${HEADERS}")
	colourized("HEADERS:\n${hs}" ${CYN})

	string(REPLACE ";" "\n" ss "${SRC}")
	colourized("SRC:\n${ss}" ${CYN})
	add_library( ${COMPONENT} STATIC ${HEADERS} ${SRC})
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
	target_compile_definitions(${COMPONENT} PRIVATE -DDATA_DIR="${PROJECT_SOURCE_DIR}/data")

	target_include_directories( ${COMPONENT} PUBLIC 
		${OpenCV_INCLUDE_DIRS}
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

	find_package(OpenMP)
	if(OpenMP_CXX_FOUND)
		target_link_libraries(${COMPONENT} PRIVATE OpenMP::OpenMP_CXX)
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
