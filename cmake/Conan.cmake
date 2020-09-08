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

  conan_cmake_run(
    REQUIRES
		opencv/4.3.0@conan/stable
		zlib/1.2.11@conan/stable
		xtensor/0.20.10@omaralvarez/public-conan
		doctest/2.3.7
		bzip2/1.0.8
    OPTIONS
		opencv:contrib=True
		opencv:nonfree=True
		opencv:gflags=False
    BASIC_SETUP
    CMAKE_TARGETS # individual targets to link to
    BUILD
    missing)
endmacro()
