cmake_minimum_required(VERSION 3.10)

project(test)
find_package(doctest REQUIRED)

file(GLOB files "*.h" "*.cpp")

add_executable(test main.cpp)
target_compile_features(test PRIVATE cxx_std_17)
target_link_libraries(test MLibs)
target_link_libraries(test doctest::doctest)
target_include_directories(test PRIVATE "/usr/include/doctest/")

include(GNUInstallDirs)
