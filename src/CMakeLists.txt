include(helpers)

# I like globs and I know they are bad practice but...
file( GLOB_RECURSE SRCS_CORE LIST_DIRECTORIES false CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/src/core/*.cpp" )
file( GLOB_RECURSE SRCS_GEO LIST_DIRECTORIES false CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/src/geometry/*.cpp" )
file( GLOB_RECURSE SRCS_UI LIST_DIRECTORIES false CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/src/ui/*.cpp" )

# headers won't affect library but will let IDEs find them
file( GLOB_RECURSE HEADERS_CORE LIST_DIRECTORIES false CONFIGURE_DEPENDS 
	"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/core/*.hpp" )
file( GLOB_RECURSE HEADERS_GEO LIST_DIRECTORIES false CONFIGURE_DEPENDS
	"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/geometry/*.hpp" )
file( GLOB_RECURSE HEADERS_UI LIST_DIRECTORIES false CONFIGURE_DEPENDS
	"${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/ui/*.hpp" )

make_component( "core" "${HEADERS_CORE}" "${SRCS_CORE}" )
#target_link_libraries(core)

make_component( "geometry" "${HEADERS_GEO}" "${SRCS_GEO}" )
target_link_libraries(geometry 
	PUBLIC 
		core)

make_component( "ui" "${HEADERS_UI}" "${SRCS_UI}" )

target_link_libraries(ui 
	PUBLIC
		core
		geometry)

if(USE_SDL2)
	find_package(SDL2 REQUIRED)
	find_package(PNG REQUIRED)
	target_link_libraries(ui
			PUBLIC
			SDL2::SDL2
			PNG::PNG)
elseif(USE_EMSCRIPTEN)
	set(EMS_FLAGS
		"SHELL:-s USE_SDL=2"
		"SHELL:-s USE_LIBPNG=1"
		"SHELL:-o gravity.html")
	set(EMS_LINKER_FLAGS
		"SHELL:-s ALLOW_MEMORY_GROWTH=1"
		"SHELL:-s ASSERTIONS=1")

	target_compile_options(ui PUBLIC
			${EMS_FLAGS})
	target_link_options(ui PUBLIC
			${EMS_LINKER_FLAGS})
	target_link_libraries(ui
			PUBLIC
			SDL2
			png)
endif()
