cmake_minimum_required(VERSION 3.8)
project(COMP-345 VERSION 0.1 DESCRIPTION "Tower Defence Game for COMP-345" LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Could also be added to CMakeSettings.json ("cmakeCommandArgs": "-DBUILD_SHARED_LIBS=OFF",)
set(BUILD_SHARED_LIBS ON)

if(WIN32)
    set(ENV{PATH} "$ENV{PATH};${CMAKE_SOURCE_DIR}/vendor/nasm-2.16.03")
    set(ENV{PATH} "$ENV{PATH};${CMAKE_SOURCE_DIR}/vendor/perl-5.40.0.1")
else()
    set(ENV{PATH} "$ENV{PATH}:${CMAKE_SOURCE_DIR}/vendor/nasm-2.16.03")
    set(ENV{PATH} "$ENV{PATH}:${CMAKE_SOURCE_DIR}/vendor/perl-5.40.0.1")
endif() 

add_subdirectory(vendor)
add_subdirectory(src)

# Install the executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Install the assets folder
install(
    DIRECTORY src/assets
    DESTINATION assets
    FILES_MATCHING PATTERN "*.*"  # Copy all files
)

# Always copy assets folder to build directory
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    DEPENDS ${CMAKE_SOURCE_DIR}/src/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)