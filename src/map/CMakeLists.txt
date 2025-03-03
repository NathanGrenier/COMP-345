add_library(map STATIC
    Map.cpp
)

target_include_directories(map PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

# If the states module depends on the core library:
target_link_libraries(map PUBLIC vendor)