include("/usr/local/lib/cmake/ruff/conan_paths.cmake")
foreach(COMPONENT ${ruff_FIND_COMPONENTS})
	include(${CMAKE_CURRENT_LIST_DIR}/${COMPONENT}-config.cmake)
endforeach()