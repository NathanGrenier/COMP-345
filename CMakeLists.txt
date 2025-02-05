cmake_minimum_required(VERSION 3.8)
project(COMP-345)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)
add_subdirectory(src)

# Could also be added to CMakeSettings.json ("cmakeCommandArgs": "-DBUILD_SHARED_LIBS=OFF",)
set(BUILD_SHARED_LIBS OFF)