cmake_minimum_required(VERSION 3.10)

project(MLibs)

find_package(xtensor REQUIRED)
set(CMAKE_BUILD_TYPE "Debug")

file(GLOB files "*.h" "*.cpp")
add_library(MLibs SHARED ${files})

target_link_libraries(MLibs xtensor)
