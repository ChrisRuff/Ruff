macro(run_conan)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_add_remote(
    NAME
    bincrafters
    URL
    https://api.bintray.com/conan/bincrafters/public-conan)
	conan_add_remote(
		NAME
		omaralvarez 
		URL
		https://api.bintray.com/conan/omaralvarez/public-conan
		)

	# Need to install libselinux
  conan_cmake_run(
    REQUIRES
		# Overrides
		freetype/2.10.1
		glib/2.64.0@bincrafters/stable 
		harfbuzz/2.6.4@bincrafters/stable

		opencv/4.3.0@conan/stable
		xtensor/0.20.10@omaralvarez/public-conan
		sdl2/2.0.12@bincrafters/stable
		sdl2_ttf/2.0.15@bincrafters/stable
		doctest/2.3.7

    OPTIONS
		#opencv:contrib=True
		#opencv:gflags=False
		#opencv:gtk=3
		sdl2:pulse=False
		sdl2:nas=False
    BASIC_SETUP
    CMAKE_TARGETS # individual targets to link to
		SETTINGS
    BUILD
    missing
		BUILD_TYPE "Release"
		)
	
endmacro()
