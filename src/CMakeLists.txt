include(helpers)

make_component("core")
#target_link_libraries(core)

if(BUILD_IMGPROC)
	make_component( "imgproc" )
endif(BUILD_IMGPROC)

if(BUILD_GEOMETRY)
	make_component( "geometry" )
	target_link_libraries(geometry
		PUBLIC
			core)
endif(BUILD_GEOMETRY)

if(BUILD_UI)
	make_component( "ui" )
	target_link_libraries(ui
			PUBLIC
			core
			geometry
			imgproc)
	if(USE_SDL2)
		target_link_libraries(ui
				PUBLIC
				CONAN_PKG::sdl
				CONAN_PKG::libpng)
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
				png)
	endif()
endif(BUILD_UI)


if(BUILD_AI)
	make_component( "ai" )
endif(BUILD_AI)

if(BUILD_PHOTOGRAMMETRY)
	make_component( "photogrammetry" )
	target_link_libraries(photogrammetry
			PUBLIC
			CONAN_PKG::opencv)
endif()

if(BUILD_SECURITY)
	make_component("security")
	target_link_libraries(security
			PUBLIC
			CONAN_PKG::gmp
			CONAN_PKG::openssl)
endif()

if(BUILD_NLP)
	make_component("nlp")
	target_link_libraries(nlp
			PUBLIC
			CONAN_PKG::ctre
	)
endif()
