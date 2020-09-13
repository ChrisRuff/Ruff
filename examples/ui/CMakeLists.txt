add_subdirectory(circles)
target_include_directories(circles SYSTEM PRIVATE ${CONAN_INCLUDE_DIRS_OPENCV})
target_compile_options( circles PRIVATE ${OPTS} )

add_subdirectory(gravity)
target_compile_options( gravity PRIVATE ${OPTS} )
target_include_directories(gravity SYSTEM PRIVATE ${CONAN_INCLUDE_DIRS_OPENCV})

add_subdirectory(astar)
target_compile_options( astar PRIVATE ${OPTS} )
target_include_directories(astar SYSTEM PRIVATE ${CONAN_INCLUDE_DIRS_OPENCV})

add_subdirectory(matching)
target_compile_options( matching PRIVATE ${OPTS} )
target_include_directories(matching SYSTEM PRIVATE ${CONAN_INCLUDE_DIRS_OPENCV})

