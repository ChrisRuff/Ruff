add_subdirectory(circles)
target_include_directories(circles SYSTEM PRIVATE 
	${CONAN_INCLUDE_DIRS_OPENCV}
	${CONAN_INCLUDE_DIRS_SDL2}
	${CONAN_INCLUDE_DIRS_SDL2_TTF}
	${CONAN_INCLUDE_DIRS_XTENSOR})
target_compile_options( circles PRIVATE ${OPTS} )

add_subdirectory(gravity)
target_compile_options( gravity PRIVATE ${OPTS} )
target_include_directories(gravity SYSTEM PRIVATE
	${CONAN_INCLUDE_DIRS_OPENCV}
	${CONAN_INCLUDE_DIRS_SDL2}
	${CONAN_INCLUDE_DIRS_SDL2_TTF}
	${CONAN_INCLUDE_DIRS_XTENSOR})

add_subdirectory(astar)
target_compile_options( astar PRIVATE ${OPTS} )
target_include_directories(astar SYSTEM PRIVATE
	${CONAN_INCLUDE_DIRS_OPENCV}
	${CONAN_INCLUDE_DIRS_SDL2}
	${CONAN_INCLUDE_DIRS_SDL2_TTF}
	${CONAN_INCLUDE_DIRS_XTENSOR})

add_subdirectory(matching)
target_compile_options( matching PRIVATE ${OPTS} )
target_include_directories(matching SYSTEM PRIVATE
	${CONAN_INCLUDE_DIRS_OPENCV}
	${CONAN_INCLUDE_DIRS_SDL2}
	${CONAN_INCLUDE_DIRS_SDL2_TTF}
	${CONAN_INCLUDE_DIRS_XTENSOR})

