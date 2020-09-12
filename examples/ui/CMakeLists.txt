cmake_minimum_required(VERSION 3.16.5)
add_subdirectory(circles)
target_compile_options( circles PRIVATE ${OPTS} )

add_subdirectory(gravity)
target_compile_options( gravity PRIVATE ${OPTS} )

add_subdirectory(astar)
target_compile_options( astar PRIVATE ${OPTS} )

add_subdirectory(matching)
target_compile_options( matching PRIVATE ${OPTS} )

